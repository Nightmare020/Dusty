// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Dusty/SaveGame/SaveGameInterface.h"
#include "CinematicTrigger.generated.h"

// Forward Declaration
class ACinematicsManager;
class ADustyCharacter;
class ACinematicsManager;
class UBoxCinematicComponent;
class ULevelSequence;
class UCinematicsDataAsset;

UCLASS()
class DUSTY_API ACinematicTrigger : public AActor, public ISaveGameInterface
{
	GENERATED_BODY()
	
public:	
	ACinematicTrigger();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/* Begin overlap function to save game when player overlaps checkpoint */
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Trigger Cinematic")
		FString CinematicName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient, DisplayName = "Cinematic Trigger Box", Category = "Trigger Cinematic")
		UBoxCinematicComponent* CollisionMesh;

	UPROPERTY(EditAnywhere, Transient, BlueprintReadWrite, Category = "Cinematic Data")
		UCinematicsDataAsset* CinematicDataAsset;

	virtual void LoadActor();

	virtual void OnActorSaved_Implementation();
private:

	UPROPERTY(Transient)
		ADustyCharacter* m_PlayerCharacter;
};
