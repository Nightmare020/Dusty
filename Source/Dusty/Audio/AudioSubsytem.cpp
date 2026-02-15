// Fill out your copyright notice in the Description page of Project Settings.


#include "AudioSubsytem.h"

#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"

void UAudioSubsytem::Initialize(FSubsystemCollectionBase& Collection)
{
	
}

void UAudioSubsytem::PlaySound2D(USoundBase* _sound)
{
	UGameplayStatics::PlaySound2D(GetWorld(), _sound);
}

void UAudioSubsytem::PlaySound3D(USoundBase* _sound, FVector _location)
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), _sound, _location);
}

void UAudioSubsytem::PlayMetasound(UAudioComponent* _sound, FVector _location)
{
	_sound->SetTriggerParameter("PlaySound");
}

void UAudioSubsytem::StopMetasound(UAudioComponent* _sound)
{
	_sound->SetTriggerParameter("StopSound");
}

void UAudioSubsytem::PauseMetasound(UAudioComponent* _sound)
{
	_sound->SetPaused(true);
}

void UAudioSubsytem::ResumeMetasound(UAudioComponent* _sound)
{
	_sound->SetPaused(false);
}

