// Fill out your copyright notice in the Description page of Project Settings.


#include "MovablePlatform.h"

#include "Dusty/Utils/DustyUtils.h"
#include "Dusty/SaveGame/DustySaveGame.h"
#include "Dusty/DustyGameInstance.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "Components/AudioComponent.h"
#include "Dusty/Audio/AudioSubsytem.h"

AMovablePlatform::AMovablePlatform()
{
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("Root"));
	mesh = CreateDefaultSubobject<UStaticMeshComponent>("PlatformMesh");
	mesh->SetupAttachment(RootComponent);
	PrimaryActorTick.bCanEverTick = true;

	m_audio = CreateDefaultSubobject<UAudioComponent>("Audio");
	m_audio->SetupAttachment(RootComponent);
}

void AMovablePlatform::BeginPlay()
{
	Super::BeginPlay();
	initPos = GetActorLocation();
	m_Destination = initPos + finalPos.RotateAngleAxis(GetActorRotation().Yaw, FVector::UpVector);


}

void AMovablePlatform::Activate()
{
	if (DelayToActivate <= 0)
	{
		m_activated = true;
	}
	else
	{
		m_currentDelayToActivate = DelayToActivate;
	}

	if(m_audio  && !m_soundPlayed)
	{
		GetGameInstance()->GetSubsystem<UAudioSubsytem>()->PlayMetasound(m_audio, GetActorLocation());
		m_soundPlayed = true;
	}
}

void AMovablePlatform::Deactivate()
{
	m_activated = false;
	if(m_audio)
	{
		GetGameInstance()->GetSubsystem<UAudioSubsytem>()->PlayMetasound(m_audio, GetActorLocation());
		m_soundPlayed = true;
	}
}

void AMovablePlatform::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
	if (m_currentDelayToActivate > 0)
	{
		m_currentDelayToActivate -= deltaTime;

		if (m_currentDelayToActivate <= 0)
		{
			m_activated = true;
		}
	}

	if(m_activated)
	{
		FVector NewPos;
		if (m_AcceleratedMovement)
		{
			NewPos = FMath::VInterpTo(GetActorLocation(), m_Destination, deltaTime, speed);
		}
		else
		{
			NewPos = FMath::VInterpConstantTo(GetActorLocation(), m_Destination, deltaTime, constantSpeed);
		}
		if ((GetActorLocation() - finalPos).Size() > 0.2f)
		{
			SetActorLocation(NewPos);
		}
		
		if ((GetActorLocation() - m_Destination).Size() < m_soundOffset  && m_soundPlayed)
		{
			GetGameInstance()->GetSubsystem<UAudioSubsytem>()->StopMetasound(m_audio);
			m_soundPlayed = false;
		}
	}

	else
	{
		FVector NewPos;
		if (m_AcceleratedMovement)
		{
			NewPos = FMath::VInterpTo(GetActorLocation(), initPos, deltaTime, speed);
		}
		else
		{
			NewPos = FMath::VInterpConstantTo(GetActorLocation(), initPos, deltaTime, constantSpeed);
		}
		if ((GetActorLocation() - initPos).Size() > 0.2f)
		{
			SetActorLocation(NewPos);
			// GetGameInstance()->GetSubsystem<UAudioSubsytem>()->StopMetasound(m_audio);
			// m_soundPlayed = false;
		}

		if ((GetActorLocation() - initPos).Size() < m_soundOffset && m_soundPlayed)
		{
			GetGameInstance()->GetSubsystem<UAudioSubsytem>()->StopMetasound(m_audio);
			m_soundPlayed = false;
		}
	}
}

void AMovablePlatform::LoadActor()
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
	Ar << m_activated;
}

void AMovablePlatform::OnActorSaved_Implementation()
{
	FActorSaveData ActorData;

	ActorData.ActorName = GetFName();
	ActorData.LevelName = GetLevel()->GetOuter()->GetFName();
	ActorData.Transform = GetActorTransform();
	FMemoryWriter MemWriter(ActorData.ByteData);

	FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
	Ar.ArIsSaveGame = true;
	// Convert binary array back into actor's variables
	Ar << m_activated;

	GetGameInstance<UDustyGameInstance>()->AddActorSaveData(ActorData);
}
