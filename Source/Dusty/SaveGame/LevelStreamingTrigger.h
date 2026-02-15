// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelStreamingTrigger.generated.h"

class UBoxComponent;
class UDustySaveGame;

UCLASS()
class DUSTY_API ALevelStreamingTrigger : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	UBoxComponent* m_Trigger;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* m_ClickablePlaceholder;

	UDustySaveGame* m_SaveGameInstance;

public:	
	// Sets default values for this actor's properties
	ALevelStreamingTrigger();

	UPROPERTY(EditInstanceOnly, Category = "Design | Level Streaming")
	TArray<FName> LevelsToLoad;

	UPROPERTY(EditInstanceOnly, Category = "Design | Level Streaming")
	TArray<FName> LevelsToUnload;

	UPROPERTY(EditInstanceOnly, Category = "Design | Level Streaming")
	bool UnloadActors = true;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	UFUNCTION()
	void LoadActorsFromLevel();

	UFUNCTION()
	void UnloadActorsFromLevel();

	TQueue<FName> LevelsToLoadQueue;

	TQueue<FName> LevelsToUnloadQueue;

};
