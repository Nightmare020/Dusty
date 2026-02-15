// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "Bench.generated.h"

/**
 * 
 */

// Forward declarations
class ADustyCharacter;
class UCinematicsDataAsset;
class UBoxCinematicComponent;

UCLASS()
class DUSTY_API ABench : public AInteractable
{
	GENERATED_BODY()
	
private:

	UPROPERTY(EditAnywhere, meta = (MakeEditWidget = true))
		FVector m_DustyPosition;

	UPROPERTY(EditAnywhere, meta = (MakeEditWidget = true))
		FTransform m_CheckPointCameraTransform;

	UPROPERTY(EditAnywhere, Transient)
		UBoxCinematicComponent* CollisionMesh;

public:
	ABench();
	virtual void Tick(float DeltaTime) override;

	/** Function to interact with object */
	void Interact() override;


	/** Function to do the start actions between Dusty and the bench (play animation, save game, etc...) */
	void SitInBench();

	/** Play the bench cinematic */
	void PlayCinematicBench();

	virtual void OnFinishedAutoMovement() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAnimMontage* SitAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Cinematic")
		FString CinematicName = "Bench";

	UPROPERTY(EditAnywhere, Transient, BlueprintReadWrite, Category = "Cinematic Data")
		UCinematicsDataAsset* CinematicDataAsset;
};
