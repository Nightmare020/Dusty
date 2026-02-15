#include "InteractiveFlower.h"

#include "NiagaraFunctionLibrary.h"
#include "../../Utils/DustyUtils.h"
#include "Animation/AnimSequence.h"
#include "Dusty/SaveGame/DustySaveGame.h"
#include "Dusty/DustyGameInstance.h"
#include "Dusty/Audio/AudioSubsytem.h"
#include "Dusty/Camera/InGameCamera/EventCameras/CameraFocusToFlowerActor.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "Dusty/InteractiveObject/Interactable/Interactable.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "Dusty/Character/DustyCharacter.h"
#include "Components/PoseableMeshComponent.h"
#include "Components/BoxComponent.h"

AInteractiveFlower::AInteractiveFlower()
{
	PrimaryActorTick.bCanEverTick = true;
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("Root"));

	Petals = CreateDefaultSubobject<USkeletalMeshComponent>("Petals");
	Petals->SetupAttachment(GetRootComponent());

	m_Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	m_Trigger->SetupAttachment(RootComponent);
}

void AInteractiveFlower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	m_Timer += DeltaTime;
	m_Timer = FMath::Clamp(m_Timer, 0.f, Duration);
	m_CurrentRootsValue = FMath::Lerp(m_StartRootsValue, m_FinishRootsValue, m_Timer / Duration) * m_ScaleFactor;

	for (int i = 0; i < ControlledRoots.Num(); ++i)
	{
		ControlledRoots[i]->UpdateRoots(m_CurrentRootsValue);
	}

	if (m_Timer >= Duration)
	{
		SetActorTickEnabled(false);
		m_Timer = 0;
		Close = false;

		if (m_CurrentRootsValue < m_Extended)
		{
			for (int i = 0; i < ControlledRoots.Num(); ++i)
			{
				ControlledRoots[i]->SetActorHiddenInGame(true);
				ControlledRoots[i]->SetActorEnableCollision(false);
			}
		}
	}
}

void AInteractiveFlower::Activate(bool _Activate)
{
	DustyUtils::HideDoublePromptHud(GetWorld());
	// if there is a camera associated and the flower wasn't waiting
	if (FocusCameraToFLower != nullptr && m_IsWaitingForCamera == false)
	{
		// call the camera and make the flower activate wait
		m_IsWaitingForCamera = true;
		
		FocusCameraToFLower->SetActivateValue(_Activate);
		FocusCameraToFLower->Init();
	}
	// if there is a camera associated and was waiting or there is no flower camera associated or there is a event camera associated
	else if (FocusCameraToFLower && m_IsWaitingForCamera || !FocusCameraToFLower)
	{
		m_IsWaitingForCamera = false;
		Super::Activate(_Activate);
		
		SetActorTickEnabled(true);
		CanBeActivatedByPlayer = false;
		m_Timer = 0.f;

		if (!_Activate)
		{			
			CanBeActivatedByPlayer = false;
			Lighted = false;

			m_StartRootsValue = m_Extended;
			m_FinishRootsValue = m_Retracted;
		}

		Close = true;

		for (int i = 0; i < InteractablesToActivate.Num(); i++)
		{
			InteractablesToActivate[i]->Activate();
		}
	}

	if(m_sound && !m_ignoreAction)
	{
		UAudioSubsytem* audioSubsystem = GetGameInstance()->GetSubsystem<UAudioSubsytem>();
		audioSubsystem->PlaySound3D(m_sound, GetActorLocation());
	}
	m_ignoreAction = false;
	
	if(m_material)
	{
		m_material->SetScalarParameterValue("Position", 5);
	}
}

void AInteractiveFlower::Activate(bool _bActivation, AActor* _actor)
{
	Activate(_bActivation);

	if(m_effect)
	{
		if(ADustyCharacter* dusty = Cast<ADustyCharacter>(_actor))
		{
			m_niagaraEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), m_effect, dusty->PoleSkeletal->GetBoneLocationByName(BallPoleBoneName, EBoneSpaces::WorldSpace), FRotator(0, 0, 0),FVector(1,1,1), true);
			if(m_lightSound)
			{
				GetGameInstance()->GetSubsystem<UAudioSubsytem>()->PlaySound2D(m_lightSound);
			}
		}
		else
		{
			m_niagaraEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), m_effect, _actor->GetActorLocation(), FRotator(0, 0, 0),FVector(1,1,1), true);
		}
		m_niagaraEffect->SetVariableVec3(FName("Origin"), GetActorLocation());
		m_niagaraEffect->SetVariableFloat(FName("Lifetime"), 0.5f);
	}
	

}

void AInteractiveFlower::BeginPlay()
{
	Close = false;
	Super::BeginPlay();

	Camera = FindComponentByClass<UCameraComponent>();

	if (Lighted)
	{
		SetActorTickEnabled(false);
		CanBeActivatedByPlayer = true;
	}

	m_CurrentRootsValue = m_Extended;
	m_StartRootsValue = m_Extended;
	m_FinishRootsValue = m_Retracted;

	m_material = UMaterialInstanceDynamic::Create(Petals->GetMaterial(0), this);
	if(m_material)
	{
		Petals->SetMaterial(0, m_material);
	}
	


}

void AInteractiveFlower::LoadActor()
{
	FActorSaveData ActorSaveData = GetGameInstance<UDustyGameInstance>()->GetActorSaveData(GetFName(), GetLevel()->GetOuter()->GetFName());

	if (ActorSaveData.Transform.GetLocation() == FVector::ZeroVector)
	{
		return;
	}
	SetActorTransform(ActorSaveData.Transform);
	FMemoryReader MemReader(ActorSaveData.ByteData);
	FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
	Ar.ArIsSaveGame = true;
	// Convert binary array back into actor's variables
	Ar << Lighted;

	if (!Lighted)
	{
		Close = true;
		CanBeActivatedByPlayer = false;
		for (int i = 0; i < ControlledRoots.Num(); ++i)
		{
			ControlledRoots[i]->SetActorHiddenInGame(true);
			ControlledRoots[i]->SetActorEnableCollision(false);
		}
	}
}

void AInteractiveFlower::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if(ADustyCharacter* Character = Cast<ADustyCharacter>(OtherActor))
	{
		if (Lighted && Character->GetLightPoleSkill())
		{
			DustyUtils::ShowDoublePromptHud(GetWorld(), m_FirstButtonId, m_SecondButtonId);
		}
	}
}

void AInteractiveFlower::NotifyActorEndOverlap(AActor* OtherActor)
{
	if (ADustyCharacter* Character = Cast<ADustyCharacter>(OtherActor))
	{
		if (Lighted && Character->GetLightPoleSkill())
		{
			DustyUtils::HideDoublePromptHud(GetWorld());
		}
	}
}
