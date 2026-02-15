// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingsSubsystem.h"
#include "../Audio/AudioSubsytem.h"
#include "../Character/DustyCharacter.h"
#include "Dusty/SaveGame/DustySaveGame.h"
#include "Dusty/DustyGameInstance.h"
#include "../Utils/DustyUtils.h"

#pragma region Display Mode

void USettingsSubsystem::SetDisplayMode(FString _DisplayModeValue)
{
	DisplayModeValue = _DisplayModeValue;
}

#pragma endregion

#pragma region Resolution

void USettingsSubsystem::SetResolution(int WidthResolutionValue, int HeightResolutionValue)
{
	Resolution = FIntPoint(WidthResolutionValue, HeightResolutionValue);

}

#pragma endregion

#pragma region Sound Volume

void USettingsSubsystem::SetGeneralVolume(int32 GeneralVolumeValue)
{
	GeneralVolume = GeneralVolumeValue;
}

void USettingsSubsystem::SetMusicVolume(int32 MusicVolumeValue)
{
	MusicVolume = MusicVolumeValue;
}

void USettingsSubsystem::SetEffectsVolume(int32 EffectsVolumeValue)
{
	EffectsVolume = EffectsVolumeValue;
}

#pragma endregion

#pragma region Camera

void USettingsSubsystem::SetCameraSensitivity(int32 CameraSensitivityValue)
{
	m_CameraSensitivity = CameraSensitivityValue;
}

void USettingsSubsystem::SetAimCameraSensitivity(int32 AimCameraSensitivityValue)
{
	m_AimCameraSensitivity = AimCameraSensitivityValue;
}

void USettingsSubsystem::SetSettingsChangesApplied(bool ChangesApplied, bool ShouldSaveSettings)
{
	m_ChangesApllied = ChangesApplied; 

	UGameUserSettings* Settings = UGameUserSettings::GetGameUserSettings();


	// Get the Dusty character
	ADustyCharacter* dusty = DustyUtils::GetDustyCharacter();
;
	if (dusty)
	{
		//// Set the music volume
		//dusty->SetSoundVolume(AudioClasses::Music, MusicVolume / 100.f);
		//// Set the music volume
		//dusty->SetSoundVolume(AudioClasses::Effects, EffectsVolume / 100.f);
		//// Set the general volume
		//dusty->SetAllSoundVolume(GeneralVolume / 100.f);

		float finalMusicVolume = (GeneralVolume / 100.f) * (MusicVolume / 100.f);
		float finalEffectsVolume = (GeneralVolume / 100.f) * (EffectsVolume / 100.f);

		dusty->SetSoundVolume(AudioClasses::Music, finalMusicVolume);
		dusty->SetSoundVolume(AudioClasses::Effects, finalEffectsVolume);
	}


	if (DisplayModeValue.Equals("Windowed", ESearchCase::IgnoreCase))
	{
		Settings->SetFullscreenMode(EWindowMode::Windowed);
	}
	else if (DisplayModeValue.Equals("Windowed Fullscreen", ESearchCase::IgnoreCase))
	{
		Settings->SetFullscreenMode(EWindowMode::WindowedFullscreen);
	}
	else if (DisplayModeValue.Equals("Fullscreen", ESearchCase::IgnoreCase))
	{
		Settings->SetFullscreenMode(EWindowMode::Fullscreen);
	}


	Settings->SetScreenResolution(Resolution);

	FSettingsSaveData SettingsSaveData(DisplayModeValue, Resolution, GeneralVolume, MusicVolume, EffectsVolume, m_CameraSensitivity, m_AimCameraSensitivity);

	UDustyGameInstance* GameInstance = Cast<UDustyGameInstance>(GetGameInstance());
	if (GameInstance && ShouldSaveSettings)
	{
		GameInstance->SetSettingsSaveData(SettingsSaveData);
		GameInstance->SaveSettings();
	}



	Settings->ApplySettings(false);	
}

#pragma endregion

#pragma region Accesibility

#pragma endregion