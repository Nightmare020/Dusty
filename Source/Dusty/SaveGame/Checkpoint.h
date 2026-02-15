// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Dusty/SaveGame/SaveGameInterface.h"
#include "Checkpoint.generated.h"

/**
 * 
 */

class AMazeActor;
class AStartMenuCamera;
class UBoxComponent;
class ADustyCharacter;
class UArrowComponent;
class AAudioActor;

UCLASS()
class DUSTY_API ACheckpoint : public AActor, public ISaveGameInterface
{
	GENERATED_BODY()

public:
	ACheckpoint(); // Constructor

	/* Begin overlap function to save game when player overlaps checkpoint */
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "m_CameraTransitionTime")
		float m_CameraTransitionTime = 1;
	UPROPERTY(EditAnywhere, meta = (MakeEditWidget = true))
		UArrowComponent* m_CheckPointCameraArrowTransform;
	UPROPERTY(EditAnywhere, meta = (MakeEditWidget = true))
		UArrowComponent* m_DustyArrowTransform;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* m_ClickablePlaceholder;

	UPROPERTY(EditAnywhere)
		bool m_IsInitialTrigger = false;
	UPROPERTY(EditAnywhere)
		UBoxComponent* CollisionMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "AudioActor")
	FString audioActor;

	/** Player Character */
	UPROPERTY(Transient)
		ADustyCharacter* m_PlayerCharacter;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "OverrideHasBeingLoaded")
	bool m_OverrideHasBeingLoaded = false;

	bool m_HasBeingLoaded = false;
	bool m_Enabled = true;

	void DoSaveGame();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "MazeActorsToDisable")
		TArray<AMazeActor*> m_MazeActorsToDisable;

	UPROPERTY(EditAnywhere)
		FVector m_InitialPositionForDustyInGame = FVector(6094.410175f, -38945.565191f, 574.703354f);


	virtual void LoadActor();
	virtual void OnActorSaved_Implementation();

};
