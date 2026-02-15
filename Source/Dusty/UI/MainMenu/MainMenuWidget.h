// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../../SaveGame/Checkpoint.h"
#include "../../SaveGame/DustySaveGame.h"
#include "MainMenuWidget.generated.h"

// Forward declarations
class UImage;
class UCanvasPanel;
class UButton;
class ADustyCharacter;
class ACheckpoint;
class UDustyGameInstance;

UCLASS()
class DUSTY_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

#pragma region MainMenuCanvasVariables
public:
	/** Canvas containing the start menu functionality */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UCanvasPanel* MainMenuCanvas;

#pragma endregion

#pragma region MainMenuButtonsVariables
public:
	//Pause Buttons
	UPROPERTY(BlueprintReadWrite, Category = "MainMenu", meta = (BindWidget))
		UButton* NewGameButton;

	UPROPERTY(BlueprintReadWrite, Category = "MainMenu", meta = (BindWidget))
		UButton* ContinueButton;

	UPROPERTY(BlueprintReadWrite, Category = "MainMenu", meta = (BindWidget))
		UButton* SettingsButton;

	UPROPERTY(BlueprintReadWrite, Category = "MainMenu", meta = (BindWidget))
		UButton* ExitButton;

#pragma endregion

#pragma region MainMenuLogic
private:
	/* Binding buttons to actions functions */
	void NativeConstruct() override;

public:
	/* Open and close main menu logic */
	void OpenMainMenu();
	void SetFocusToSettingsButton();
	void CloseMainMenu();

	bool GetIsInMainMenu() { return m_IsInMainMenu; }
	
	UFUNCTION(BlueprintCallable)
		void SetIsInMainMenu(bool IsInMainMenu) { m_IsInMainMenu = IsInMainMenu; }

#pragma endregion

#pragma region MenuButtonsFunctions
private:
	// This functions checks if we have to enable the Continue Button
	void UpdateContinueButtonState();

	/** Dusty Character variables */
	APlayerController* PlayerController;
	ADustyCharacter* DustyCharacter;

	/* Variable to check if we're in the main menu */
	UPROPERTY()
		bool m_IsInMainMenu = true;

public:
	//One function per button
	UFUNCTION()
	void B_Continue();

	UFUNCTION()
	void B_Settings();

	UFUNCTION()
	void B_NewGame();

	UFUNCTION()
	void B_Quit();

	UFUNCTION()
	void PlayButtonSound();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Sound Button")
	USoundBase* m_buttonSound;

#pragma endregion
private:
	TArray<AActor*> m_audioActors;
};