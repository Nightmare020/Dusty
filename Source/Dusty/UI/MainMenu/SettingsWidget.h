// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingsWidget.generated.h"

// Forward declaration
class UCanvasPanel;
class UComboBoxString;
class USlider;
class ADustyCharacter;
class UTextBlock;
class UButton;

/**
 * 
 */
UCLASS()
class DUSTY_API USettingsWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	/* Open and close settings menu logic */
	void OpenSettingsMenu();
	void CloseSettingsMenu();

	/** Canvas containing the settings menu functionality */
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidget))
		UCanvasPanel* SettingsMenuCanvas;

	/** Canvas containing the exit settings menu functionality when we press to cancel */
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidget))
		UCanvasPanel* AreYouSureMenu;

	/** Press No button */
	UPROPERTY(BlueprintReadWrite, Category = "MainMenu", meta = (BindWidget))
		UButton* NoButton;

	/** Press Yes button */
	UPROPERTY(BlueprintReadWrite, Category = "MainMenu", meta = (BindWidget))
		UButton* YesButton;

#pragma region Display Mode

public:
	UFUNCTION()
		void OnDisplayModeLeftButtonClicked();

	UFUNCTION()
		void OnDisplayModeRightButtonClicked();

	/** Select display mode buttons */
	UPROPERTY(BlueprintReadWrite, Category = "MainMenu", meta = (BindWidget))
		UButton* DisplayModeLeftButton;

	UPROPERTY(BlueprintReadWrite, Category = "MainMenu", meta = (BindWidget))
		UButton* DisplayModeRightButton;

	/** Text Box with the possible different game display modes */
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidget))
		UTextBlock* DisplayModeText;

private:
	UFUNCTION()
		void DisplayModeSetting();

	UFUNCTION()
		void ChangeDisplayMode(bool IsRightButton);

	/** Array for all the possible display modes */
	TArray<FString> m_DisplayModes = { "Windowed", "Fullscreen", "Windowed Fullscreen" };

	/** Index for the current display mode */
	int32 m_CurrentDisplayMode = 1; // Fullscreen display mode as default display mode

#pragma endregion

#pragma region Resolution

public:

	UFUNCTION()
	void OnResolutionLeftButtonClicked();

	UFUNCTION()
	void OnResolutionRightButtonClicked();

	/** Select display mode buttons */
	UPROPERTY(BlueprintReadWrite, Category = "MainMenu", meta = (BindWidget))
		UButton* ResolutionLeftButton;

	UPROPERTY(BlueprintReadWrite, Category = "MainMenu", meta = (BindWidget))
		UButton* ResolutionRightButton;

	/** Text Box with the possible different game resolutions */
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidget))
		UTextBlock* ResolutionText;

private:
	UFUNCTION()
		void ResolutionSetting();

	UFUNCTION()
		void ChangeResolution(bool IsRightButton);

	/** Array for all the possible resolutions */
	TArray<FString> m_Resolutions = { "1280 x 720", "1366 x 768", "1920 x 1080", "2560 x 1440", "1560 x 1080", "3840 x 2160", "7680 x 4320", "5120 x 1440" };

	/** Index for the current display mode */
	int32 m_CurrentResolution = 2; // 1920 x 1080 as default resolution

#pragma endregion

#pragma region Sound Volume

public:

	UFUNCTION()
		void OnGeneralVolumeChanged(const float Value);

	UFUNCTION()
		void OnMusicVolumeChanged(const float Value);

	UFUNCTION()
		void OnEffectsVolumeChanged(const float Value);

	/** Canvas containing the start menu functionality */
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidget))
		USlider* SliderGeneralSound;

	/** Text Box with the possible different game resolutions */
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidget))
		UTextBlock* GeneralSoundValue;

	/** Canvas containing the start menu functionality */
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidget))
		USlider* SliderMusicSound;

	/** Text Box with the possible different game resolutions */
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidget))
		UTextBlock* MusicSoundValue;

	/** Canvas containing the start menu functionality */
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidget))
		USlider* SliderEffectsSound;

	/** Text Box with the possible different game resolutions */
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidget))
		UTextBlock* EffectSoundValue;

#pragma endregion

#pragma region Camera

public:
	UFUNCTION()
		void OnCameraSensitivityChanged(const float Value);

	/** Canvas containing the start menu functionality */
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidget))
		USlider* SliderCameraSensitivity;

	/** Text Box with the possible different game resolutions */
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidget))
		UTextBlock* CameraSensitivityValue;

#pragma endregion

#pragma region Accesibility

#pragma endregion

public:
	//Getters & Setter
	bool GetIsInSettingsMenu() { return m_IsInSettingsMenu; }
	void SetIsInSettingsMenu(bool NewIsInSettingsMenu) { m_IsInSettingsMenu = NewIsInSettingsMenu; }

	bool GetWasInBookMenu() { return m_WasInBookMenu; }
	void SetWasInBookMenu(bool NewWasInBookMenu) { m_WasInBookMenu = NewWasInBookMenu; }

	UFUNCTION()
		void OnApplyChangesButtonPressed();

	UFUNCTION()
		void OnCancelButtonPressed();

	/** Button to apply the changes done to settings */
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidget))
		UButton* ApplyChangesButton;

	/** Button to cancel changes done to settings */
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidget))
		UButton* CancelButton;

private:
	/* Binding buttons to actions functions */
	void NativeConstruct() override;

	/** Execute while menu is open, every tick */
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

	/** Transform float value to integer */
	int32 FloatSliderValueToInt(const float Value);

	// These functions are used to cahe the original variables and restore if we cancel the settings
	void CacheCurrentSettings();
	void RestoresOriginalSettings();

	/** Dusty Character variables */
	APlayerController* PlayerController;
	ADustyCharacter* DustyCharacter;

	/* Variable to check if we're in the main menu */
	UPROPERTY()
		bool m_IsInSettingsMenu = false;

	/* Variable to check if we're in the main menu */
	UPROPERTY()
		bool m_WasInBookMenu = false;

	/** This values are cached as the original settings values in ase we cancel the apply settings operation */
	FString m_OriginalDisplayMode;
	FString m_OriginalResolution;
	int32 m_OriginalGeneralVolume;
	int32 m_OriginalMusicVolume;
	int32 m_OriginalEffectsVolume;
	int32 m_OriginalCameraSensitivity;
};
