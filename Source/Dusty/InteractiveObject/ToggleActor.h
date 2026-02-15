// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../SaveGame/SaveGameInterface.h"
#include "ToggleActor.generated.h"


USTRUCT()
struct FEmissionDirections
{
	GENERATED_BODY()

	FVector EmissionDirection;
	AToggleActor* HittedActor;

};

UCLASS()
class DUSTY_API AToggleActor : public AActor, public ISaveGameInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AToggleActor();

	UPROPERTY(EditAnywhere, Category = "Design | Toggle Actor")
	bool CanBeActivatedByPlayer = true;

#pragma region ControlFunctions
	virtual void Activate();
	virtual void Activate(bool _bActivation);
	virtual void Activate(bool _bActivation, AActor* _actor);
	virtual void Deactivate();

	/** Activate function that can be implemented in blueprints. Does not need a declaration in the cpp file */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Toggle Actor", meta = (DisplayName = "Dusty_Activate"))
	void BP_Activate();

	virtual void Reset() {};
#pragma endregion
};
