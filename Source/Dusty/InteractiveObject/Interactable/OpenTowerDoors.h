// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dusty/InteractiveObject/Interactable/Interactable.h"
#include "../../SaveGame/SaveGameInterface.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Dusty/Utils/DustyConst.h"
#include "OpenTowerDoors.generated.h"

class AMazeTriggerTargetOverrider;
class AGlyphController;
class USphereComponent;
class ADissolveDoor;
UCLASS()
class DUSTY_API AOpenTowerDoors : public AInteractable, public ISaveGameInterface
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly)
	USphereComponent* m_KeyControllerTrigger;

	UPROPERTY(EditAnywhere)
	TArray<ADissolveDoor*> Doors;

	UPROPERTY(EditAnywhere, meta = (MakeEditWidget = true))
	FVector m_DustyPosition;

	UPROPERTY(SaveGame)
	bool m_Unlocked = false;

public:

	AOpenTowerDoors();
	virtual void Activate(bool _bActivation = true) override;
	virtual void Deactivate() override;

	UPROPERTY(EditAnywhere)
		int m_HudButtonId = 4;
	/* Begin overlap function to save game when player overlaps interactable object area */
	void NotifyActorBeginOverlap(AActor* OtherActor) override;
	void NotifyActorEndOverlap(AActor* OtherActor) override;

	UPROPERTY(EditAnywhere)
	float TimeExitKeyAnimation = 5.f;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Interact() override;

	virtual void OnFinishedAutoMovement() override;


private:
	UPROPERTY(EditAnywhere)
	bool m_Activated = true;
	bool m_started = false;

	float m_currentTimeExitKeyAnimation = 0;

public:
	UPROPERTY(EditInstanceOnly, meta=(Comment="Si no te llamas lucas no lo toques."))
	AMazeTriggerTargetOverrider* TargetOverrider = nullptr;

};
