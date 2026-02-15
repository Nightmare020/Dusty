// Fill out your copyright notice in the Description page of Project Settings.


#include "BoxCinematicComponent.h"
#include "../Cinematics/CinematicsSubsystem.h"
#include "../UI/HUD/HUDSubsystem.h"

void UBoxCinematicComponent::SkipCinematic()
{
	// Get Cinematic Subsystem
	UCinematicsSubsystem* cinematicSubsystem = GetOwner()->GetGameInstance()->GetSubsystem<UCinematicsSubsystem>();

	// Get HUD Subsystem
	UHUDSubsystem* hudSubsystem = GetOwner()->GetGameInstance()->GetSubsystem<UHUDSubsystem>();

	if (cinematicSubsystem->IsCinematicPlaying)
	{
		hudSubsystem->EvaluateSkipCinematic();
	}
}