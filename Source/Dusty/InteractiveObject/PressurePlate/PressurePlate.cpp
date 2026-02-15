// Fill out your copyright notice in the Description page of Project Settings.


#include "PressurePlate.h"
#include "Components/ShapeComponent.h"
#include "PressurePlateController.h"
#include "Dusty/SaveGame/DustySaveGame.h"
#include "Dusty/DustyGameInstance.h"
#include "Dusty/Audio/AudioSubsytem.h"
#include "NiagaraComponent.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "Dusty/Character/DustyCharacter.h"

APressurePlate::APressurePlate()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	PlateMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plate"));
	PlateMesh->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
	NiagaraCmp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara Effect"));
	NiagaraCmp->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);

	GetCollisionComponent()->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
}

void APressurePlate::LoadActor()
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
	Ar << m_SomethingOnTop;
	if (m_SomethingOnTop > 0)
	{
		SetActorTickEnabled(true);
	}
}

void APressurePlate::OnActorSaved_Implementation()
{
	FActorSaveData ActorData;

	ActorData.ActorName = GetFName();
	ActorData.LevelName = GetLevel()->GetOuter()->GetFName();
	ActorData.Transform = GetActorTransform();
	FMemoryWriter MemWriter(ActorData.ByteData);

	FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
	Ar.ArIsSaveGame = true;
	// Convert binary array back into actor's variables
	Ar << m_SomethingOnTop;

	GetGameInstance<UDustyGameInstance>()->AddActorSaveData(ActorData);
}

void APressurePlate::BeginPlay()
{
	Super::BeginPlay();
	LoadActor();
	m_OriginPos = PlateMesh->GetRelativeLocation();
	m_ActivatePos = FVector(m_OriginPos.X, m_OriginPos.Y, m_OriginPos.Z - PressedDis);
	m_IsActive = false;
	if (ControllerInstance)
	{
		ControllerInstance->IncreasePiece();
	}
	if (AdditionalControllerInstance)
	{
		AdditionalControllerInstance->IncreasePiece();
	}
}

void APressurePlate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (m_SomethingOnTop > 0)
	{
		m_SpeedTValue += DeltaTime * m_PressSpeed;
		FVector NewPos = FMath::Lerp(PlateMesh->GetRelativeLocation(), m_ActivatePos, m_SpeedTValue);
		PlateMesh->SetRelativeLocation(NewPos);
		if (m_SpeedTValue >= 0.95f)
		{
			if (!m_IsActive)
			{
				if (m_DustyPressingPressurePlate)
				{
					ADustyCharacter* dusty = DustyUtils::GetDustyCharacter();
					dusty->UnlockAchievement(TEXT("Speedrun_ACH"));
				}
				Activated();
			}
			SetActorTickEnabled(false);
		}
	}
	else
	{
		m_SpeedTValue += DeltaTime * m_PressSpeed;
		FVector NewPos = FMath::Lerp(PlateMesh->GetRelativeLocation(), m_OriginPos, m_SpeedTValue);
		PlateMesh->SetRelativeLocation(NewPos);
		if (m_SpeedTValue >= 0.95f)
		{
			if (m_IsActive)
			{
				Deactivated();
			}
			SetActorTickEnabled(false);
		}
	}
}

void APressurePlate::NotifyActorBeginOverlap(AActor* OtherActor)
{
	//Play sound
	if(m_pressSound && m_SomethingOnTop <= 0)
	{
		GetGameInstance()->GetSubsystem<UAudioSubsytem>()->PlaySound3D(m_pressSound, GetActorLocation());
	}
	++m_SomethingOnTop;
	m_SpeedTValue = 0;
	SetActorTickEnabled(true);

	if (m_IsMachinePressurePlate)
	{
		if (ADustyCharacter* dusty = Cast<ADustyCharacter>(OtherActor))
		{
			m_DustyPressingPressurePlate = true;
			DustyUtils::PrintMessage("True");
		}
	}
}

void APressurePlate::NotifyActorEndOverlap(AActor* OtherActor)
{
	if (m_IsActive || m_SomethingOnTop > 0)
	{
		--m_SomethingOnTop;
		m_SpeedTValue = 0;
		//Play sound
		if(m_pressSound)
		{
			GetGameInstance()->GetSubsystem<UAudioSubsytem>()->PlaySound3D(m_pressSound, GetActorLocation());
		}
		SetActorTickEnabled(true);
	}
	if (m_IsMachinePressurePlate)
	{
		if (ADustyCharacter* dusty = Cast<ADustyCharacter>(OtherActor))
		{
			m_DustyPressingPressurePlate = false;
			DustyUtils::PrintMessage("False");
		}
	}
}

void APressurePlate::Activated()
{
	m_IsActive = true;
	NiagaraCmp->Activate(true);

	if (ControllerInstance)
	{
		ControllerInstance->APieceIsSolved();
	}
	if (AdditionalControllerInstance)
	{
		AdditionalControllerInstance->APieceIsSolved();
	}
}

void APressurePlate::Deactivated()
{
	m_IsActive = false;
	SetActorTickEnabled(false);
	if (ControllerInstance)
	{
		ControllerInstance->IncreasePiece();
	}
	if (AdditionalControllerInstance)
	{
		AdditionalControllerInstance->IncreasePiece();
	}
}

