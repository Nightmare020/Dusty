// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AudioSubsytem.generated.h"

/**
 * 
 */

enum AudioClasses
{
	Music,
	Effects
};


class USoundBase;
UCLASS()
class DUSTY_API UAudioSubsytem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	UFUNCTION()
	void PlaySound2D(USoundBase* _sound);

	UFUNCTION()
	void PlaySound3D(USoundBase* _sound, FVector _location);
	
	UFUNCTION()
	void PlayMetasound(UAudioComponent* _sound, FVector _location);
	UFUNCTION()
	void StopMetasound(UAudioComponent* _sound);
	UFUNCTION()
	void PauseMetasound(UAudioComponent* _sound);
	UFUNCTION()
	void ResumeMetasound(UAudioComponent* _sound);
	
	/** Getters and Setters for sound volume types*/
	float GetGeneralVolume() { return m_GeneralVolume; }
	void SetGeneralVolume(float NewGeneralVolume) { m_GeneralVolume = NewGeneralVolume; }

	float GetMusicVolume() { return m_MusicVolume; }
	void SetMusicVolume(float NewMusicVolume) { m_MusicVolume = NewMusicVolume; }

	float GetSFXVolume() { return m_EffectsVolume; }
	void SetSFXVolume(float NewSFXVolume) { m_EffectsVolume = NewSFXVolume; }

	float GetAmbientVolume() { return m_AmbientVolume; }
	void SetAmbientVolume(float NewAmbientVolume) { m_AmbientVolume = NewAmbientVolume; }

private:
	// General Master Volume Value
	float m_GeneralVolume;

	// Music Volume Value
	float m_MusicVolume;

	// SFX Volume Value
	float m_EffectsVolume;

	// Ambient Volume Value
	float m_AmbientVolume;

};
