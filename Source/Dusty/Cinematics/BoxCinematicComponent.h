// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "BoxCinematicComponent.generated.h"

/**
 * 
 */
UCLASS()
class DUSTY_API UBoxCinematicComponent : public UBoxComponent
{
	GENERATED_BODY()

public:

	/** Get and Set flag for cinematic to check if it has already played */
	bool GetCinematicHasPlayed() { return m_HasCinematicBeenPlayed; }
	void SetCinematicPlayed(bool HasCinematicPlayed) { m_HasCinematicBeenPlayed = HasCinematicPlayed; }
	
	/** Evaluate Skip Cinematic Interaction if we're playing the cinematic */
	void SkipCinematic();

	/** Get Transform after cinematic if necessary */
	FTransform GetPlayerTransformAfterCinematic() { return m_PlayerAfterCinematicTransform; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cinematic Flags")
		bool IsPlayerRelevantInCinematic = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cinematic Flags")
		bool IsInGameCinematic = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cinematic Flags")
		bool IsShotsFragmentedCinematic = false;

private:
	/** Flag to control wether the cinematic for this trigger has been played */
	bool m_HasCinematicBeenPlayed = false;

	UPROPERTY(EditAnywhere, meta = (MakeEditWidget = true))
		FTransform m_PlayerAfterCinematicTransform;
};
