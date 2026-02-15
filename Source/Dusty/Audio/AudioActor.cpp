// Fill out your copyright notice in the Description page of Project Settings.


#include "AudioActor.h"

#include "AudioSubsytem.h"
#include "Components/AudioComponent.h"

// Sets default values
AAudioActor::AAudioActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_sound = CreateDefaultSubobject<UAudioComponent>("Audio Component");
	m_sound->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AAudioActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAudioActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAudioActor::PlaySound()
{
	
	if(m_sound)
	{
		if(!m_isPaused)
		{
			GetGameInstance()->GetSubsystem<UAudioSubsytem>()->PlayMetasound(m_sound, FVector::ZeroVector);
		}

		else
		{
			GetGameInstance()->GetSubsystem<UAudioSubsytem>()->ResumeMetasound(m_sound);
		}
		
	}
}

void AAudioActor::StopSound()
{
	UAudioSubsytem* audioSubsystem = GetGameInstance()->GetSubsystem<UAudioSubsytem>();
	if(audioSubsystem && m_sound)
	{
		audioSubsystem->StopMetasound(m_sound);
	}
}

void AAudioActor::PauseSound()
{
	GetGameInstance()->GetSubsystem<UAudioSubsytem>()->PauseMetasound(m_sound);
	m_isPaused = true;
}

void AAudioActor::PlayTrigger(const FName& _parameter)
{
	UAudioSubsytem* audioSubsystem = GetGameInstance()->GetSubsystem<UAudioSubsytem>();
	if(audioSubsystem && m_sound)
	{
		m_sound->SetTriggerParameter(_parameter);
	}
}

