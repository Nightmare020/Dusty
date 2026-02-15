#include "DissolveActorController.h"

#include "DissolveActor.h"
#include "../Key/PlanetsKeyController.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Components/PoseableMeshComponent.h"
#include "Dusty/Audio/AudioSubsytem.h"
#include "Dusty/Camera/InGameCamera/EventCameras/EventCamera.h"
#include "Dusty/Character/DustyCharacter.h"


ADissolveActorController::ADissolveActorController()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("Root")));
	
	ControlBox = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	ControlBox->SetupAttachment(RootComponent);
	ControlSphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
	ControlSphere->SetupAttachment(ControlBox);
	SpehereDownPos = CreateDefaultSubobject<USceneComponent>(TEXT("SphereDownPos"));
	SpehereDownPos->SetupAttachment(ControlBox);
}

void ADissolveActorController::Activate()
{
	// function created to use the stored activate value
	if (m_IfCameraReadyWithActor)
	{
		Activate(m_StoredActivateValue, m_StoredActor);
	}
	else if (m_IfCameraReady)
	{
		Activate(m_StoredActivateValue);
	}

	// reset values to default
	m_IfCameraReadyWithActor = false;
	m_IfCameraReady = false;
	m_StoredActivateValue = false;
	m_StoredActor = nullptr;
}


void ADissolveActorController::BeginPlay()
{
	UMaterialInterface* baseMat = ControlBox->GetMaterial(0);
	m_dynamicMaterial = UMaterialInstanceDynamic::Create(baseMat, this);
	ControlBox->SetMaterial(0, m_dynamicMaterial);
	ControlSphere->SetMaterial(0, m_dynamicMaterial);

	Super::BeginPlay();

	UpdateMaterialsAndSpherePhysics();

	m_sphereInitialPos = ControlSphere->GetRelativeLocation();

	for (int i = 0; i < DissolveEntities.Num(); i++)
	{
		if (DissolveEntities[i])
		{
			DissolveEntities[i]->Init(Lighted, Duration);
		}
	}

	if (!Lighted)
	{
		ControlSphere->SetRelativeLocation(SpehereDownPos->GetRelativeLocation());
	}
}

void ADissolveActorController::UpdateMaterialsAndSpherePhysics()
{
	if (Lighted)
	{
		m_dynamicMaterial->SetScalarParameterValue(KeyBaseParam, 0.f);
	}
	else
	{
		m_dynamicMaterial->SetScalarParameterValue(KeyBaseParam, 2.f);
	}
}

void ADissolveActorController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_sphereGoingUp)
	{
		FVector newPos = FMath::Lerp(ControlSphere->GetRelativeLocation(), m_sphereInitialPos, DeltaTime);
		ControlSphere->SetRelativeLocation(newPos);

		if (FVector(ControlSphere->GetRelativeLocation() - m_sphereInitialPos).GetAbs().Size() <= 0.2f)
		{
			ControlSphere->SetRelativeLocation(m_sphereInitialPos);
			m_sphereGoingUp = false;
			SetActorTickEnabled(false);
		}
	}
	else if (m_sphereGoingDown)
	{
		FVector newPos = FMath::Lerp(ControlSphere->GetRelativeLocation(), SpehereDownPos->GetRelativeLocation(), DeltaTime);
		ControlSphere->SetRelativeLocation(newPos);

		if (FVector(ControlSphere->GetRelativeLocation() - SpehereDownPos->GetRelativeLocation()).GetAbs().Size() <= 0.2f)
		{
			ControlSphere->SetRelativeLocation(SpehereDownPos->GetRelativeLocation());
			m_sphereGoingDown = false;
			SetActorTickEnabled(false);
		}
	}
}

void ADissolveActorController::Activate(bool _bActivation)
{
	if (m_EventCamera && !m_IfCameraReady)
	{
		m_EventCamera->Init(m_IsFollowUpEvent);
		m_IfCameraReady = true;
		m_StoredActivateValue = _bActivation;
	}
	else
	{
		if (Lighted != _bActivation)
		{

			Lighted = _bActivation;
			for (int i = 0; i < DissolveEntities.Num(); i++)
			{
				if (DissolveEntities[i])
				{
					DissolveEntities[i]->Activate(_bActivation);
				}
			}

			if (_bActivation)
			{
				m_sphereGoingUp = true;
				m_sphereGoingDown = false;
			}
			else
			{
				m_sphereGoingUp = false;
				m_sphereGoingDown = true;
			}
		}
		
		SetActorTickEnabled(true);
		UpdateMaterialsAndSpherePhysics();
	}
}

void ADissolveActorController::Activate(bool _bActivation, AActor* _actor)
{
	this->Activate(_bActivation);

	if (m_EventCamera && !m_IfCameraReadyWithActor)
	{
		m_IfCameraReadyWithActor = true;
		m_StoredActor = _actor;
	}
	else
	{
		if(_bActivation && m_niagaraSystem)
		{
			if(ADustyCharacter* dusty = Cast<ADustyCharacter>(_actor))
			{
				m_niagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), m_niagaraSystem, ControlSphere->GetComponentLocation(), FRotator(0, 0, 0), FVector(3, 3, 3), true);
				m_niagaraComponent->SetVariableVec3(FName("Origin"), dusty->PoleSkeletal->GetBoneLocationByName(BallPoleBoneName, EBoneSpaces::WorldSpace));
				m_niagaraComponent->SetVariableFloat(FName("Lifetime"), 1.0f);
			}
			else
			{
				m_niagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), m_niagaraSystem, ControlSphere->GetComponentLocation(), FRotator(0, 0, 0), FVector(3, 3, 3), true);
				m_niagaraComponent->SetVariableVec3(FName("Origin"), _actor->GetActorLocation());
				m_niagaraComponent->SetVariableFloat(FName("Lifetime"), 1.0f);
			}
			if(m_particleSound)
			{
				GetGameInstance()->GetSubsystem<UAudioSubsytem>()->PlaySound2D(m_particleSound);
			}
		}

		else if(!_bActivation && m_niagaraSystem)
		{
			if(ADustyCharacter* dusty = Cast<ADustyCharacter>(_actor))
			{
				m_niagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), m_niagaraSystem, dusty->PoleSkeletal->GetBoneLocationByName(BallPoleBoneName, EBoneSpaces::WorldSpace), FRotator(0, 0, 0), FVector(3, 3, 3), true);
				m_niagaraComponent->SetVariableVec3(FName("Origin"), ControlSphere->GetComponentLocation());
				m_niagaraComponent->SetVariableFloat(FName("Lifetime"), 1.0f);
			}
			else
			{
				m_niagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), m_niagaraSystem, _actor->GetActorLocation(), FRotator(0, 0, 0), FVector(3, 3, 3), true);
				m_niagaraComponent->SetVariableVec3(FName("Origin"), ControlSphere->GetComponentLocation());
				m_niagaraComponent->SetVariableFloat(FName("Lifetime"), 1.0f);
			}
			if(m_particleSound)
			{
				GetGameInstance()->GetSubsystem<UAudioSubsytem>()->PlaySound2D(m_particleSound);
			}
		}
	}
	
}

