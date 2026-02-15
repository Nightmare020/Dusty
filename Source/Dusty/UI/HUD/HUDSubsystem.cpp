// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDSubsystem.h"
#include "PlayerHUDWidget.h"
#include "AimPointerHUD.h"
#include "InteractiveButtonInterface.h"
#include <Components/TextBlock.h>
#include "../../Character/DustyCharacter.h"
#include "Components/PoseableMeshComponent.h"
#include "../../UI/HUD/PlayerHUDWidget.h"
#include "../../Cinematics/CinematicsSubsystem.h"
#include "../../InteractiveObject/Light/LightActor.h"
#include "../../InteractiveObject/Light/Magnetism/MagneticCube.h"
#include "../../InteractiveObject/Interactable/InteractiveFlower.h"
#include "../../InteractiveObject/Interactable/DissolveActorController.h"
#include "../Book/Book3D/BookActorTextures.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CanvasPanel.h"
#include "Components/ProgressBar.h"
#include "../../Utils/DustyUtils.h"
#include "../../Utils/DustyConst.h"
#include "CommonActionWidget.h"
#include "Dusty/UI/DustyUserWidget.h"



void UHUDSubsystem::EvaluateSkipCinematic()
{
#if WITH_EDITOR
	// Initialize player
	m_PlayerCharacter = Cast<ADustyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	// Get the precent of the bar for the skip cinematic
	float CurrentBarPercent = m_PlayerCharacter->GetHUDWidget()->SkipCinematicHoldBar->GetPercent();

	if (m_PlayerCharacter->SkipButtonPressed)
	{
		// Show the skip button canvas if it was hidden
		if (!m_PlayerCharacter->GetHUDWidget()->IsSkipCinematicCanvasVisible())
		{
			ShowSkipCinematicMessage();
		}

		// Increment the progress bar
		CurrentBarPercent += SkipCinematicBarSpeed * GetWorld()->GetDeltaSeconds();

		// If the timer is active and the skip button is pressed, cancel timer
		if (GetWorld()->GetTimerManager().IsTimerActive(m_SkipCinematicTimerHandle))
		{
			GetWorld()->GetTimerManager().ClearTimer(m_SkipCinematicTimerHandle);
		}

		if (CurrentBarPercent >= 1.f)
		{
			// Get Cinematic Subsystem
			UCinematicsSubsystem* cinematicSubsystem = GetGameInstance()->GetSubsystem<UCinematicsSubsystem>();

			// Reset the progress bar
			CurrentBarPercent = 1.f;

			m_PlayerCharacter->SkipButtonPressed = false;
			HideSkipCinematicMessage();
			cinematicSubsystem->EndCinematic();

			// Set bar percent to 0
			CurrentBarPercent = 0.f;
		}
	}
	else
	{
		// Decrease the percentage of the progress bar
		CurrentBarPercent -= SkipCinematicBarSpeed * GetWorld()->GetDeltaSeconds();

		// Reset the progress bar if button is not pressed and it gets to 0
		if (CurrentBarPercent <= 0.f)
		{
			// Make sure the percentage don't get lower to 0
			CurrentBarPercent = 0.f;

			// Hide the HUD with a timer when the bar reaches 0 and the skip button is not pressed
			if (!GetWorld()->GetTimerManager().IsTimerActive(m_SkipCinematicTimerHandle))
			{
				GetWorld()->GetTimerManager().SetTimer(m_SkipCinematicTimerHandle, this, &UHUDSubsystem::HideSkipCinematicMessage, 7.f, false);
			}
		}
	}

	// Update the progress bar percentage
	m_PlayerCharacter->GetHUDWidget()->SkipCinematicHoldBar->SetPercent(CurrentBarPercent);

#endif
}

void UHUDSubsystem::ShowSkipCinematicMessage()
{
#if WITH_EDITOR
	// Initialize player
	m_PlayerCharacter = Cast<ADustyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	m_PlayerCharacter->GetHUDWidget()->SkipCinematicCanvasVisible();
#endif
}

void UHUDSubsystem::HideSkipCinematicMessage()
{
#if WITH_EDITOR
	// Initialize player
	m_PlayerCharacter = Cast<ADustyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	m_PlayerCharacter->GetHUDWidget()->SkipCinematicCanvasHidden();
#endif
}
