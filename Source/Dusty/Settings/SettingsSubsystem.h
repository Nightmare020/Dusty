// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameFramework/GameUserSettings.h"
#include "SettingsSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class DUSTY_API USettingsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
#pragma region Display Mode

public:
	// Static function to change the display mode
	void SetDisplayMode(FString DisplayModeValue);
private:
	FString DisplayModeValue;
#pragma endregion

#pragma region Resolution

public:
	// Static function to change the resolution
	void SetResolution(int WidthResolutionValue, int HeightResolutionValue);
private:
	FIntPoint Resolution;
#pragma endregion

#pragma region Sound Volume

public:
	// Static function to change the general volume
	void SetGeneralVolume(int32 GeneralVolumeValue);

	// Static function to change the music volume
	void SetMusicVolume(int32 MusicVolumeValue);

	// Static function to change the effects volume
	void SetEffectsVolume(int32 EffectsVolumeValue);
private:
	int GeneralVolume;
	int MusicVolume;
	int EffectsVolume;
#pragma endregion

#pragma region Camera
public:
	// Change the camera sensitivity
	void SetCameraSensitivity(int32 AmbientVolumeValue);

	// Get Camera Sensitivity value in float value
	float GetCameraSensitivity() { return m_CameraSensitivity / 100.f; }
	// Change the camera sensitivity
	void SetAimCameraSensitivity(int32 AmbientVolumeValue);

	// Get Camera Sensitivity value in float value
	float GetAimCameraSensitivity() { return m_AimCameraSensitivity / 100.f; }

private:
	/** Camera Sensitivity Value for Player Character */
	int32 m_CameraSensitivity = 100;
	int32 m_AimCameraSensitivity = 100;

#pragma endregion


#pragma region Accesibility

#pragma endregion

public:
	// Get and set if changes are applied to the settings
	bool GetSettingsChangesApplied() { return m_ChangesApllied; }
	void SetSettingsChangesApplied(bool ChangesApplied, bool ShouldSaveSettings = true);

private:
	/** This flag checks if the settings changes have been applied */
	bool m_ChangesApllied = false;
};
