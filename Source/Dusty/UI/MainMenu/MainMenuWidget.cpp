// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"
#include "../../Character/DustyCharacter.h"
#include "../../SaveGame/Checkpoint.h"
#include "../../DustyGameInstance.h"
#include "../../Cinematics/CinematicsSubsystem.h"
#include "../../Character/CharacterStates/ActionsFilter.h"
#include "SettingsWidget.h"
#include "Components/AudioComponent.h"

//Placeholder for B_Quit() function
#include "Dusty/Audio/AudioActor.h"
#include "Dusty/Audio/AudioSubsytem.h"
#include "Dusty/Character/DustyMovementComponent.h"
#include "Kismet\KismetSystemLibrary.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	/* Obtain Dusty character */
	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	DustyCharacter = Cast<ADustyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	// Main Menu Buttons
	ContinueButton->OnClicked.AddDynamic(this, &UMainMenuWidget::B_Continue);
	ContinueButton->OnHovered.AddDynamic(this, &UMainMenuWidget::PlayButtonSound);
	NewGameButton->OnClicked.AddDynamic(this, &UMainMenuWidget::B_NewGame);
	NewGameButton->OnHovered.AddDynamic(this, &UMainMenuWidget::PlayButtonSound);
	SettingsButton->OnClicked.AddDynamic(this, &UMainMenuWidget::B_Settings);
	SettingsButton->OnHovered.AddDynamic(this, &UMainMenuWidget::PlayButtonSound);
	ExitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::B_Quit);
	ExitButton->OnHovered.AddDynamic(this, &UMainMenuWidget::PlayButtonSound);

	// Check if Continue button should be enabled
	UpdateContinueButtonState();

	// Check where we should focus depending if the COntinue button is enabled or not
	if (ContinueButton->GetIsEnabled())
	{
		ContinueButton->SetFocus();
	}
	else
	{
		NewGameButton->SetFocus();
	}

	//mainMenuSound = CreateDefaultSubobject<UAudioComponent>("main menu audio");
	m_buttonSound = LoadObject<USoundBase>(nullptr, TEXT("/Game/Audio/Tracks/Book/Seleccionar.Seleccionar"));
}

void UMainMenuWidget::OpenMainMenu()
{
	m_IsInMainMenu = true;

	MainMenuCanvas->SetVisibility(ESlateVisibility::Visible);
	PlayerController->SetInputMode(FInputModeGameAndUI());
	PlayerController->bShowMouseCursor = true;

	// Change player character state to Main Menu state
	DustyCharacter->ActionsFilterComponent->SetCurrentState(UMainMenuState::StaticClass());
	UpdateContinueButtonState();

	
	//Lo siento... esta es la unica forma que he encontrado para gestionar desde un menu los audios. No se hace en cada tick asi que no me aniquilen :)
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAudioActor::StaticClass(), m_audioActors);
	//Play main menu sound
	if(m_audioActors.Num() > 0)
	{
		for(AActor* actor : m_audioActors)
		{
			if(actor->GetName() == "BP_AudioActor_C_1")
			{
				AAudioActor* audio = Cast<AAudioActor>(actor);
				audio->PlaySound();
			}
		}
	}
	
}

void UMainMenuWidget::SetFocusToSettingsButton()
{
	SettingsButton->SetFocus();
}

void UMainMenuWidget::CloseMainMenu()
{
	m_IsInMainMenu = false;

	PlayerController->SetPause(false);
	PlayerController->SetInputMode(FInputModeGameOnly());
	PlayerController->bShowMouseCursor = false;

	MainMenuCanvas->SetVisibility(ESlateVisibility::Hidden);

	// Get Cinematic Subsystem
	UCinematicsSubsystem* cinematicSubsystem = GetGameInstance()->GetSubsystem<UCinematicsSubsystem>();

	if (!DustyCharacter->GetSettingsMenuWidget().GetIsInSettingsMenu())
	{
		if (cinematicSubsystem->IsCinematicPlaying)
		{
			DustyCharacter->ActionsFilterComponent->SetCurrentState(UCinematicState::StaticClass());
		}
		else
		{
			DustyCharacter->ActionsFilterComponent->SetCurrentState(UGroundMovementState::StaticClass());
		}
	}

	//Stop Main menu sound
	UDustySaveGame* SaveGameInstance = nullptr;
	DustyUtils::PrintMessage("Search Save Game", FColor::Yellow, 5);
	if (UGameplayStatics::DoesSaveGameExist(SAVE_SLOT_NAME, 0))
	{
		DustyUtils::PrintMessage("Save Game Exists", FColor::Yellow, 5);
		SaveGameInstance = Cast<UDustySaveGame>(UGameplayStatics::LoadGameFromSlot(SAVE_SLOT_NAME, 0));
		DustyUtils::PrintMessage("Dusty Audio: ", SaveGameInstance->PlayerSavedData.audioName,  FColor::Yellow, 5);
	}

	else
	{
		DustyUtils::PrintMessage("Save Game Doesn't Exists", FColor::Yellow, 5);
	}


	if(m_audioActors.Num() > 0)
	{
		for(AActor* actor : m_audioActors)
		{
			DustyUtils::PrintMessage(actor->GetName(), FColor::Yellow, 5);
			if(actor->GetName() == "BP_AudioActor_C_1")
			{
				AAudioActor* audio = Cast<AAudioActor>(actor);
				audio->StopSound();
			}

			if (SaveGameInstance != nullptr && actor->GetName() == SaveGameInstance->PlayerSavedData.audioName)
			{
				DustyUtils::PrintMessage("Audio Saved found", FColor::Yellow, 5);
				AAudioActor* audio = Cast<AAudioActor>(actor);
				audio->PlaySound();
				DustyCharacter->SetCurrentSound(audio);
			}
		}
	}
	//if (DustyCharacter->GetCurrentAudio())
	//{
	//	DustyCharacter->GetCurrentAudio()->PlaySound();
	//}
	
}

void UMainMenuWidget::UpdateContinueButtonState()
{
	if (UGameplayStatics::DoesSaveGameExist(SAVE_SLOT_NAME, 0))
	{
		ContinueButton->SetIsEnabled(true);
	}
	else
	{
		ContinueButton->SetIsEnabled(false);
	}
}

void UMainMenuWidget::B_Continue()
{
	CloseMainMenu();

	// Initialize character
	DustyCharacter->ExitStartCamera();
	DustyCharacter->FirstTimeOpeningBook = false;
	DustyCharacter->DustyMovementComponent->Activate();
	GetGameInstance<UDustyGameInstance>()->LoadStartingLevelsAgain();

}

void UMainMenuWidget::B_NewGame()
{
	// Set the flag indicating that New Game was pressed
	UDustyGameInstance* GameInstance = GetGameInstance<UDustyGameInstance>();
	
	if (GameInstance)
	{
		CloseMainMenu();
		GameInstance->StartNewGame();
		DustyCharacter->ExitStartCamera();
		DustyCharacter->DustyMovementComponent->Activate();
	}
}

void UMainMenuWidget::B_Settings()
{
	DustyCharacter->ShowSettingsInMainMenu();
	SetVisibility(ESlateVisibility::Hidden);
}

void UMainMenuWidget::B_Quit()
{
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->ConsoleCommand("quit");
}

void UMainMenuWidget::PlayButtonSound()
{
	if(m_buttonSound)
	{
		GetGameInstance()->GetSubsystem<UAudioSubsytem>()->PlaySound2D(m_buttonSound);
	}
	
}
