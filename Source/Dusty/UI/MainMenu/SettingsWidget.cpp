// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingsWidget.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "Components/ComboBoxString.h"
#include "Components/Button.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "../../Character/DustyCharacter.h"
#include "../../Character/CharacterStates/ActionsFilter.h"
#include "../../Settings/SettingsSubsystem.h"
#include "../../UI/Book/Book3D/BookActorTextures.h"
#include "../../UI/HUD/PlayerHUDWidget.h"
#include "MainMenuWidget.h"

void USettingsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	/* Obtain Dusty character */
	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	DustyCharacter = Cast<ADustyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	// Get the settings subsystem
	USettingsSubsystem* settingsSubsystem = GetGameInstance()->GetSubsystem<USettingsSubsystem>();

	// Settings Menu options
	DisplayModeLeftButton->OnClicked.AddDynamic(this, &USettingsWidget::OnDisplayModeLeftButtonClicked);
	DisplayModeRightButton->OnClicked.AddDynamic(this, &USettingsWidget::OnDisplayModeRightButtonClicked);
	ResolutionLeftButton->OnClicked.AddDynamic(this, &USettingsWidget::OnResolutionLeftButtonClicked);
	ResolutionRightButton->OnClicked.AddDynamic(this, &USettingsWidget::OnResolutionRightButtonClicked);
	SliderGeneralSound->OnValueChanged.AddDynamic(this, &USettingsWidget::OnGeneralVolumeChanged);
	SliderMusicSound->OnValueChanged.AddDynamic(this, &USettingsWidget::OnMusicVolumeChanged);
	SliderEffectsSound->OnValueChanged.AddDynamic(this, &USettingsWidget::OnEffectsVolumeChanged);
	SliderCameraSensitivity->OnValueChanged.AddDynamic(this, &USettingsWidget::OnCameraSensitivityChanged);
	ApplyChangesButton->OnClicked.AddDynamic(this, &USettingsWidget::OnApplyChangesButtonPressed);
	CancelButton->OnClicked.AddDynamic(this, &USettingsWidget::OnCancelButtonPressed);

	// Initialize the display mode and resolution texts
	DisplayModeText->SetText(FText::FromString(m_DisplayModes[m_CurrentDisplayMode]));
	ResolutionText->SetText(FText::FromString(m_Resolutions[m_CurrentResolution]));
	GeneralSoundValue->SetText(FText::FromString(FString::FromInt(100)));
	MusicSoundValue->SetText(FText::FromString(FString::FromInt(100)));
	EffectSoundValue->SetText(FText::FromString(FString::FromInt(100)));
	CameraSensitivityValue->SetText(FText::FromString(FString::FromInt(100)));
}

void USettingsWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (m_IsInSettingsMenu)
	{
		//if (DustyCharacter && DustyCharacter->ReturnButtonPressed)
		//{
		//	CloseSettingsMenu();
		//	if (m_WasInBookMenu)
		//	{
		//		DustyCharacter->GetBookActor().OpenBook(LG_Bookmarks::LogBook);
		//		m_WasInBookMenu = false;
		//	}
		//	else
		//	{
		//		DustyCharacter->GetMainMenuWidget().OpenMainMenu();
		//	}

		//	m_IsInSettingsMenu = false;
		//	DustyCharacter->ReturnButtonPressed = false;
		//}
	}
}

void USettingsWidget::OpenSettingsMenu()
{
	// Cache the current settings when the menu is opened
	CacheCurrentSettings();

	SettingsMenuCanvas->SetVisibility(ESlateVisibility::Visible);

	PlayerController->SetInputMode(FInputModeGameAndUI());
	PlayerController->bShowMouseCursor = true;

	m_IsInSettingsMenu = true;

	ApplyChangesButton->SetFocus();
}

void USettingsWidget::CloseSettingsMenu()
{
	PlayerController->SetPause(false);
	PlayerController->SetInputMode(FInputModeGameOnly());
	PlayerController->bShowMouseCursor = false;

	m_IsInSettingsMenu = false;

	SettingsMenuCanvas->SetVisibility(ESlateVisibility::Hidden);
}

#pragma region Display Mode

void USettingsWidget::OnDisplayModeLeftButtonClicked()
{
	// Change display mode in text
	ChangeDisplayMode(false);

	// Set the new display mode in the game
	DisplayModeSetting();
}

void USettingsWidget::OnDisplayModeRightButtonClicked()
{
	// Change display mode in text
	ChangeDisplayMode(true);

	// Set the new display mode in the game
	DisplayModeSetting();
}

void USettingsWidget::DisplayModeSetting()
{
	// Get the settings subsystem
	USettingsSubsystem* settingsSubsystem = GetGameInstance()->GetSubsystem<USettingsSubsystem>();

	FString DisplayType = DisplayModeText->GetText().ToString();

	// Set the new diaplsy mode in PlayerSettings
	settingsSubsystem->SetDisplayMode(DisplayType);
}

void USettingsWidget::ChangeDisplayMode(bool IsRightButton)
{
	if (IsRightButton)
	{
		// Move to the right (next display mode)
		m_CurrentDisplayMode = (m_CurrentDisplayMode + 1) % m_DisplayModes.Num();
	}
	else
	{
		// Move to the left (previous mode)
		m_CurrentDisplayMode = (m_CurrentDisplayMode - 1 + m_DisplayModes.Num()) % m_DisplayModes.Num();
	}

	// Update the text with the new display mode
	DisplayModeText->SetText(FText::FromString(m_DisplayModes[m_CurrentDisplayMode]));
}

#pragma endregion

#pragma region Resolution

void USettingsWidget::OnResolutionLeftButtonClicked()
{
	// Change resolution in text box
	ChangeResolution(false);

	// Set the new resolution in the game
	ResolutionSetting();
}

void USettingsWidget::OnResolutionRightButtonClicked()
{
	// Change resolution in text box
	ChangeResolution(true);

	// Set the new resolution in the game
	ResolutionSetting();
}

void USettingsWidget::ResolutionSetting()
{
	// Get the settings subsystem
	USettingsSubsystem* settingsSubsystem = GetGameInstance()->GetSubsystem<USettingsSubsystem>();

	// Obtain the resolutio text selected
	FString Resolution = ResolutionText->GetText().ToString();

	// Variables to store screen width and heigth
	float ResolutionWidth = 0.0f;
	float ResolutionHeigth = 0.0f;

	// Find the 'x' position and split the char
	FString LeftStr;
	FString RightStr;
	
	if (Resolution.Split(TEXT(" x "), &LeftStr, &RightStr))
	{
		// Transform parts to float
		ResolutionWidth = FCString::Atof(*LeftStr);
		ResolutionHeigth = FCString::Atof(*RightStr);
	}

	settingsSubsystem->SetResolution(ResolutionWidth, ResolutionHeigth);
}

void USettingsWidget::ChangeResolution(bool IsRightButton)
{
	if (IsRightButton)
	{
		// Move to the right (next display mode)
		m_CurrentResolution = (m_CurrentResolution + 1) % m_Resolutions.Num();
	}
	else
	{
		// Move to the left (previous mode)
		m_CurrentResolution = (m_CurrentResolution - 1 + m_Resolutions.Num()) % m_Resolutions.Num();
	}

	// Update the text with the new display mode
	ResolutionText->SetText(FText::FromString(m_Resolutions[m_CurrentResolution]));
}

#pragma endregion

#pragma region Sound Volume

void USettingsWidget::OnGeneralVolumeChanged(const float Value)
{
	// Get the settings subsystem
	USettingsSubsystem* settingsSubsystem = GetGameInstance()->GetSubsystem<USettingsSubsystem>();

	int32 GeneralVolume = FloatSliderValueToInt(Value * 100);

	// Change text value
	GeneralSoundValue->SetText(FText::FromString(FString::FromInt(GeneralVolume)));

	// Set the new general volume in the game
	settingsSubsystem->SetGeneralVolume(GeneralVolume);
}

void USettingsWidget::OnMusicVolumeChanged(const float Value)
{
	// Get the settings subsystem
	USettingsSubsystem* settingsSubsystem = GetGameInstance()->GetSubsystem<USettingsSubsystem>();

	int32 MusicVolume = FloatSliderValueToInt(Value * 100);

	// Change text value
	MusicSoundValue->SetText(FText::FromString(FString::FromInt(MusicVolume)));

	// Set the new music volume in the game
	settingsSubsystem->SetMusicVolume(MusicVolume);
}

void USettingsWidget::OnEffectsVolumeChanged(const float Value)
{
	// Get the settings subsystem
	USettingsSubsystem* settingsSubsystem = GetGameInstance()->GetSubsystem<USettingsSubsystem>();

	int32 EffectsVolume = FloatSliderValueToInt(Value * 100);

	// Change text value
	EffectSoundValue->SetText(FText::FromString(FString::FromInt(EffectsVolume)));

	// Set the new music volume in the game
	settingsSubsystem->SetEffectsVolume(EffectsVolume);
}

#pragma endregion

#pragma region Camera

void USettingsWidget::OnCameraSensitivityChanged(const float Value)
{
	// Get the settings subsystem
	USettingsSubsystem* settingsSubsystem = GetGameInstance()->GetSubsystem<USettingsSubsystem>();

	int32 CameraSensitivity = FloatSliderValueToInt(Value * 100);

	// Change text value
	CameraSensitivityValue->SetText(FText::FromString(FString::FromInt(CameraSensitivity)));

	// Set the new camera sensitivity in the game
	settingsSubsystem->SetCameraSensitivity(CameraSensitivity);
}

#pragma endregion

#pragma region Accesibility

#pragma endregion

void USettingsWidget::OnApplyChangesButtonPressed()
{

	// Mantain the changed values and close the settings menu
	CloseSettingsMenu();

	if (m_WasInBookMenu)
	{
		DustyCharacter->GetBookActor().OpenBook(LG_Bookmarks::LogBook);
		m_WasInBookMenu = false;
	}
	else
	{
		DustyCharacter->GetMainMenuWidget().OpenMainMenu();
	}
}

void USettingsWidget::OnCancelButtonPressed()
{
	// Restore the original values and close the settings menu
	RestoresOriginalSettings();
	CloseSettingsMenu();
	
	if (m_WasInBookMenu)
	{
		DustyCharacter->GetBookActor().OpenBook(LG_Bookmarks::LogBook);
		m_WasInBookMenu = false;
	}
	else
	{
		DustyCharacter->GetMainMenuWidget().OpenMainMenu();
	}
}

int32 USettingsWidget::FloatSliderValueToInt(const float Value)
{
	// Round the slider value to an integer
	int32 RoundedSliderValue = FMath::RoundToInt(Value);

	return RoundedSliderValue;
}

void USettingsWidget::CacheCurrentSettings()
{
	USettingsSubsystem* SettingsSubsystem = GetGameInstance()->GetSubsystem < USettingsSubsystem>();

	// Store the current values
	m_OriginalDisplayMode = m_DisplayModes[m_CurrentDisplayMode];
	m_OriginalResolution = m_Resolutions[m_CurrentResolution];
	m_OriginalGeneralVolume = FloatSliderValueToInt(SliderGeneralSound->GetValue() * 100);
	m_OriginalMusicVolume = FloatSliderValueToInt(SliderMusicSound->GetValue() * 100);
	m_OriginalEffectsVolume = FloatSliderValueToInt(SliderEffectsSound->GetValue() * 100);
	m_OriginalCameraSensitivity = FloatSliderValueToInt(SliderCameraSensitivity->GetValue() * 100);
}

void USettingsWidget::RestoresOriginalSettings()
{
	USettingsSubsystem* SettingsSubsystem = GetGameInstance()->GetSubsystem<USettingsSubsystem>();

	// Restore original settings values
	DisplayModeText->SetText(FText::FromString(m_OriginalDisplayMode));
	ResolutionText->SetText(FText::FromString(m_OriginalResolution));
	SliderGeneralSound->SetValue(m_OriginalGeneralVolume / 100.0f);
	GeneralSoundValue->SetText(FText::FromString(FString::FromInt(m_OriginalGeneralVolume)));
	SliderMusicSound->SetValue(m_OriginalMusicVolume / 100.0f);
	MusicSoundValue->SetText(FText::FromString(FString::FromInt(m_OriginalMusicVolume)));
	SliderEffectsSound->SetValue(m_OriginalEffectsVolume / 100.0f);
	EffectSoundValue->SetText(FText::FromString(FString::FromInt(m_OriginalEffectsVolume)));
	SliderCameraSensitivity->SetValue(m_OriginalCameraSensitivity / 100.0f);
	CameraSensitivityValue->SetText(FText::FromString(FString::FromInt(m_OriginalCameraSensitivity)));

	// Apply the original values on to the game
	SettingsSubsystem->SetDisplayMode(m_OriginalDisplayMode);

	// Split the resolution string correctly
	FString LeftStr, RightStr;
	if (m_OriginalResolution.Split(TEXT(" x "), &LeftStr, &RightStr))
	{
		// Convert strings to floats
		float ResolutionWidth = FCString::Atof(*LeftStr);
		float ResolutionHeight = FCString::Atof(*RightStr);

		// Set the original resolution
		SettingsSubsystem->SetResolution(ResolutionWidth, ResolutionHeight);
	}

	SettingsSubsystem->SetGeneralVolume(m_OriginalGeneralVolume);
	SettingsSubsystem->SetMusicVolume(m_OriginalMusicVolume);
	SettingsSubsystem->SetEffectsVolume(m_OriginalEffectsVolume);
	SettingsSubsystem->SetCameraSensitivity(m_OriginalCameraSensitivity);

}