#include "InfiniteProvider.h"

#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "Dusty/Character/DustyCharacter.h"
#include "Dusty/InteractiveObject/Light/LightActor.h"
#include "Dusty/InteractiveObject/Light/LightParticle.h"
#include "../../../Utils/DustyConst.h"
#include "../../../Utils/DustyUtils.h"
#include "Components/PoseableMeshComponent.h"
#include "NiagaraComponent.h"
#include "Dusty/Audio/AudioSubsytem.h"

void AInfiniteProvider::OnConstruction(const FTransform& Transform)
{
	if (IsDrainer)
	{
		if (m_DrainerMesh)
		{
			m_Mesh->SetStaticMesh(m_DrainerMesh);
		}
	}
	else
	{
		if (m_ProviderMesh)
		{
			m_Mesh->SetStaticMesh(m_ProviderMesh);
		}
	}
}

void AInfiniteProvider::ToggleBallLight(bool _Lighted, float _redValue, float _greenValue, float _blueValue)
{
	if (_Lighted)
	{
		if (IsDrainer)
		{
			//DustyUtils::SetEmissiveMaterialValues(m_BallMesh, BallDrainerRColor, BallDrainerGColor, BallDrainerBColor);
		}
		else
		{
			//DustyUtils::SetEmissiveMaterialValues(m_BallMesh, BallRColor, BallGColor, BallBColor);
		}
	}
	else
	{
		//DustyUtils::SetEmissiveMaterialValues(m_BallMesh, 0, 0, 0);
	}
}

void AInfiniteProvider::BallMovement(float _DeltaTime, float _distance)
{
	m_BallTimer += _DeltaTime * FMath::FRandRange(m_MinRandom, m_MaxRandom) * _distance;
	FVector newPos = DustyUtils::GetRandomMovement(m_InitialBallPosition, m_BallTimer, m_Amplitude, m_Frequency * _distance, 0);
	m_BallMesh->SetRelativeLocation(FVector(newPos.X, newPos.Y, m_InitialBallPosition.Z + m_maxBallHeight * _distance));
}

AInfiniteProvider::AInfiniteProvider()
{
	PrimaryActorTick.bCanEverTick = true;
	m_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("mesh"));
	SetRootComponent(m_Mesh);

	m_BallPosition = CreateDefaultSubobject<USceneComponent>(TEXT("BallPosition"));
	m_BallPosition->SetupAttachment(m_Mesh);

	m_BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ballMesh"));
	m_BallMesh->SetupAttachment(m_BallPosition);

	m_Trigger = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger"));
	m_Trigger->SetupAttachment(m_BallPosition);

}


void AInfiniteProvider::BeginPlay()
{
	Super::BeginPlay();
	CanBeActivatedByPlayer = false;
	m_InitialBallPosition = m_BallMesh->GetRelativeLocation();

	for (ALightActor* LightActor : m_StartingActors)
	{
		m_AffectedActors.Add(LightActor);
	}

	
	// ToggleBallLight(m_Activated, 0, 0, 0);
	// if (IsDrainer)
	// {
	// 	DustyUtils::SetEmissiveMaterialValues(m_BallMesh, 0, 0, 0);
	// }
	// else
	// {
	// 	DustyUtils::SetEmissiveMaterialValues(m_BallMesh, 0, 0, 0);
	// }
}


void AInfiniteProvider::Tick(float DeltaTime)
{
	float NormalizedDistance;
	ADustyCharacter* TempPlayer = DustyUtils::GetDustyCharacter();

	if(TempPlayer)
	{
		float distance = (GetActorLocation() - TempPlayer->GetActorLocation()).Size();
		float ClampedDistance = FMath::Clamp(distance, 150, m_Trigger->GetScaledSphereRadius());
		NormalizedDistance = 1.0f - (ClampedDistance - 150) / (m_Trigger->GetScaledSphereRadius() - 150);
	
		Super::Tick(DeltaTime);
		if (!m_Activated)
		{
			return;
		}

		BallMovement(DeltaTime, NormalizedDistance);

		if (TempPlayer && !TempPlayer->GetLightPoleSkill()) //Unlocked skill?
		{
			return;
		}
		if(m_niagaraEffect && !IsDrainer && m_Player)
		{
			particleLifetime += DeltaTime;
			m_niagaraEffect->SetWorldLocation(m_Player->PoleSkeletal->GetBoneLocationByName(BallPoleBoneName, EBoneSpaces::WorldSpace));
			if (particleLifetime >= 0.35f)
			{
				if(TempPlayer->GetPoleMaterial()->K2_GetScalarParameterValue(FName("position")) >= 0)
				{
					float newValue = FMath::Lerp(TempPlayer->GetPoleMaterial()->K2_GetScalarParameterValue(FName("position")), 0, 6.5f * DeltaTime);
					TempPlayer->GetPoleMaterial()->SetScalarParameterValue(FName("position"), newValue);
					if(particleLifetime >= 10.0f)
					{
						particleLifetime = 0;
						m_niagaraEffect = nullptr;
					}
				}
			}
		}
		else if(m_niagaraEffect)
		{
			particleLifetime += DeltaTime;
			if (particleLifetime >= 0.35f)
			{
				if(TempPlayer->GetPoleMaterial()->K2_GetScalarParameterValue(FName("position")) <= 1)
				{
					float newValue = FMath::Lerp(TempPlayer->GetPoleMaterial()->K2_GetScalarParameterValue(FName("position")), 1, 8.5f * DeltaTime);
					TempPlayer->GetPoleMaterial()->SetScalarParameterValue(FName("position"), newValue);
				}
				if(particleLifetime >= 10.0f)
				{
					particleLifetime = 0;
					m_niagaraEffect = nullptr;
				}
			}
		}
	
	
		FHitResult HitResult;
		FVector Start = m_BallPosition->GetComponentLocation();
		FCollisionQueryParams Params;

		Params.AddIgnoredActor(this);
		for (ALightActor* LightActor : m_AffectedActors)
		{
			if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, LightActor->GetActorLocation(), ECC_Visibility, Params))
			{
				if (HitResult.GetActor() == LightActor)
				{
					if(IsDrainer && LightActor->GetLightStatus())
					{
						LightActor->Activate(false, this);
					}

					else if(!IsDrainer && !LightActor->GetLightStatus())
					{
						LightActor->Activate(true, this);
					}
					
				}
			}
		}

		if (m_Player)
		{
		
			if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, m_Player->GetActorLocation(), ECC_Visibility, Params))
			{
				if (HitResult.GetActor() == m_Player)
				{
					if (!IsDrainer && (!m_Player->IsPoleLighted()) && (m_effect) && !m_niagaraEffect)
					{
						m_niagaraEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), m_effect, m_Player->PoleSkeletal->GetBoneLocationByName(BallPoleBoneName, EBoneSpaces::WorldSpace), FRotator(0, 0, 0),FVector(1,1,1), true);
						m_niagaraEffect->SetVariableVec3(FName("Origin"), m_BallPosition->GetComponentLocation());
						m_niagaraEffect->OnSystemFinished.AddDynamic(this, &AInfiniteProvider::OnNiagaraSystemFinished);
						m_niagaraEffect->SetVariableFloat(FName("Lifetime"), 0.5f);

						//Play particle sound
						if(m_poleChargedSound)
						{
							GetGameInstance()->GetSubsystem<UAudioSubsytem>()->PlaySound3D(m_poleChargedSound, GetActorLocation());
						}
						
					}
					else if ((IsDrainer) && (m_Player->IsPoleLighted())  && !m_niagaraEffect)
					{
						m_niagaraEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), m_effect, m_BallPosition->GetComponentLocation(), FRotator(0, 0, 0),FVector(1,1,1), true);
						m_niagaraEffect->SetVariableVec3(FName("Origin"), m_Player->PoleSkeletal->GetBoneLocationByName(BallPoleBoneName, EBoneSpaces::WorldSpace));
						m_niagaraEffect->OnSystemFinished.AddDynamic(this, &AInfiniteProvider::OnNiagaraSystemFinished);
						m_niagaraEffect->SetVariableFloat(FName("Lifetime"), 0.5f);
						//Play particle sound
						if(m_poleDischargeSound)
						{
							GetGameInstance()->GetSubsystem<UAudioSubsytem>()->PlaySound3D(m_poleDischargeSound, GetActorLocation());
						}
					}

					m_Player->TogglePoleLight(!IsDrainer);
					if(!IsDrainer && m_PlayerInsideTrigger)
					{
						m_Player->SetDustyTattooValue(m_Player->FullTattooValue);
					}
				}
			}
		}
	}
	if (IsDrainer)
	{
		DustyUtils::SetEmissiveMaterialValues(m_BallMesh, BallDrainerRColor + NormalizedDistance * m_lightForce, BallDrainerGColor + NormalizedDistance * m_lightForce, BallDrainerBColor + NormalizedDistance * m_lightForce);
	}
	else
	{
		DustyUtils::SetEmissiveMaterialValues(m_BallMesh, BallRColor + NormalizedDistance * m_lightForce, BallGColor + NormalizedDistance * m_lightForce, BallBColor + NormalizedDistance * m_lightForce);
	}
}

void AInfiniteProvider::SetPlayer(ADustyCharacter* Character)
{
	m_Player = Character;
}

void AInfiniteProvider::RemovePlayer()
{
	if (!m_PlayerInsideTrigger)
	{
		m_Player = nullptr;
	}
}

void AInfiniteProvider::Activate()
{
	m_Activated = true;
	ToggleBallLight(m_Activated, 0, 0, 0);
}

UStaticMeshComponent* AInfiniteProvider::GetBallMesh() const
{
	return m_BallMesh;
}

USceneComponent* AInfiniteProvider::GetBallComponent() const
{
	return m_BallPosition;
}

void AInfiniteProvider::OnNiagaraSystemFinished(UNiagaraComponent* FinishedComponent)
{
	m_niagaraEffect = nullptr;
	particleLifetime = 0;
	m_Player = nullptr;
}


void AInfiniteProvider::NotifyActorBeginOverlap(AActor* OtherActor)
{
	
	if (ALightActor* LightActor = Cast<ALightActor>(OtherActor))
	{
		m_AffectedActors.Add(LightActor);
	}
	else if (ADustyCharacter* DustyCharacter = Cast<ADustyCharacter>(OtherActor))
	{
		if (!DustyCharacter->GetLightPoleSkill()) //Unlocked skill?
		{
			return;
		}
		m_Player = DustyCharacter; 
		m_PlayerInsideTrigger = true;

	}

}


void AInfiniteProvider::NotifyActorEndOverlap(AActor* OtherActor)
{
	if (ALightActor* LightActor = Cast<ALightActor>(OtherActor))
	{
		m_AffectedActors.Remove(LightActor);
	}
	else if (ADustyCharacter* DustyCharacter = Cast<ADustyCharacter>(OtherActor))
	{
		if (!DustyCharacter->IsPoleLighted())
		{
			DustyCharacter->SetDustyTattooValue(DustyCharacter->NoTattooValue);
		}
		else
		{
			DustyCharacter->SetDustyTattooValue(DustyCharacter->PoleLightedTattooValue);
		}
		if(!m_niagaraEffect)
		{
			m_Player = nullptr;
		}
		m_PlayerInsideTrigger = false;
	}
}
