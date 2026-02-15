#include "CaterpillarNPC.h"

#include "CaterpillarSpline.h"
#include "Dusty/Utils/DustyUtils.h"
#include "Kismet/KismetMathLibrary.h"


ACaterpillarNPC::ACaterpillarNPC()
{
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("Root"));

	m_SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMesh");
	m_SkeletalMesh->SetupAttachment(RootComponent);
}

void ACaterpillarNPC::BeginPlay()
{
	Super::BeginPlay();

	SetActorTickEnabled(true);


	// material logic 
	UMaterialInterface* MaterialBase = m_SkeletalMesh->GetMaterial(0);

	DynamicMaterial = GetMaterial(MaterialBase);
	m_SkeletalMesh->SetMaterial(0, DynamicMaterial);
	 
	SetLighting(Lighted, true);

	
	//starting location and rotation of caterpillar
	if (m_Spline)
	{
		if(m_SplineStartingPoint < 0)
		{
			SetActorLocation(m_Spline->GetClosestLocation(GetActorLocation()));
		}
		else
		{
			SetActorLocation(m_Spline->GetClosestLocation(m_SplineStartingPoint));
		}
		SetActorRotation(m_Spline->GetCurrentRotation(GetActorLocation()));
	}
}

void ACaterpillarNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// if not correctly lighted keep updating
	if(!m_IsCorrectlyLighted)
	{
		UpdateLighting(DeltaTime);
	}
	
	if(m_Spline)
	{
		const float DistanceInSpline = DeltaTime * m_BaseSpeed;
		const FVector CurrentLocation = GetActorLocation();
		const FVector NewLocation = m_Spline->GetNextLocation(CurrentLocation, DistanceInSpline);
		
		SetActorLocation(NewLocation);
		SetActorRotation(m_Spline->GetCurrentRotation(NewLocation));
	}
}

void ACaterpillarNPC::Activate(bool _bActivation)
{
	// wait for it to be fully finished to give or take light again
	if(m_IsCorrectlyLighted)
	{
		// set for visual lighting on material
		SetLighting(_bActivation, false);
	}
}

void ACaterpillarNPC::UpdateLighting(float DeltaTime)
{
	float const LightDistance = abs(m_LightValueTarget-m_CurrentLightValue);
	float const FAlpha = FMath::Min(DeltaTime * m_BlendLightSpeed / LightDistance, 1.0f);
	
	m_CurrentLightValue = FMath::Lerp(m_CurrentLightValue, m_LightValueTarget, FAlpha);
	
	DynamicMaterial->SetScalarParameterValue(FName("Position"), m_CurrentLightValue);

	if(m_CurrentLightValue == m_LightValueTarget)
	{
		m_IsCorrectlyLighted = true;
		Lighted = m_LightValueTarget == m_MinLightValue;
	}
}

void ACaterpillarNPC::SetLighting(bool _newLighting, bool _IsInstant)
{
	const float TempTarget = (_newLighting) ? m_MinLightValue : m_MaxLightValue;

	// set value instantly
	if (_IsInstant)
	{
		DynamicMaterial->SetScalarParameterValue(FName("Position"), TempTarget);
		m_LightValueTarget = TempTarget;
		m_CurrentLightValue = TempTarget;
		m_IsCorrectlyLighted = true;
		return;
	}
	
	// if already in correct value
	if (m_CurrentLightValue == TempTarget)
	{
		return;
	}	
	
	if (DynamicMaterial)
	{
		// set new target and enable the lerp system
		m_LightValueTarget = TempTarget;
		m_IsCorrectlyLighted = false;
	}
}


FRotator ACaterpillarNPC::GetNewRotation() const
{
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this); // Ignore this actor in the raycast

	FVector Start = GetActorLocation();
	FVector End = GetActorLocation() - GetActorUpVector();

	DrawDebugLine(GetWorld(), Start, End, FColor::Green,false, -1.0f, 0,1.0f);
	
	if (bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams))
	{
		FRotator NewRotation = UKismetMathLibrary::MakeRotFromZ(HitResult.ImpactNormal);
		return NewRotation;
	}
	
	return FRotator::ZeroRotator;
}

UMaterialInstanceDynamic* ACaterpillarNPC::GetMaterial(UMaterialInterface* _material)
{
	DynamicMaterial = UMaterialInstanceDynamic::Create(_material, this);
	return DynamicMaterial;
}
