// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Dusty/Utils/DustyConst.h"
#include "HUDSubsystem.generated.h"

// Forward declarations
class UPlayerHUDWidget;
class AAimPointerHUD;
class UInteractiveButtonInterface;
class ADustyCharacter;
class ALightActor;
class UDustyUserWidget;

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class DUSTY_API UHUDSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:

	
	/** With an specific text font, show the save game text in the widget */
	UFUNCTION(BlueprintCallable)
		void EvaluateSkipCinematic();

	/** With an specific text font, show the save game text in the widget */
	UFUNCTION(BlueprintCallable)
		void ShowSkipCinematicMessage();
	
	/** With an specific text font, show the save game text in the widget */
	UFUNCTION(BlueprintCallable)
		void HideSkipCinematicMessage();

	
	/** Skip Cinematic bar fill speed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SkipCinematicBarSpeed = 0.5f;

private:

	
	/** Player Character */
	UPROPERTY(Transient)
		ADustyCharacter* m_PlayerCharacter;

	/** Declare a time handle variable for the hidding of the skip cinematic hud if player is not pressing the skip button */
		FTimerHandle m_SkipCinematicTimerHandle;
};
