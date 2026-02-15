// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Interactable/Interactable.h"
#include "../../SaveGame/SaveGameInterface.h"
#include "PoleKeyActor.generated.h"

class USphereComponent;
class UNiagaraComponent;

UCLASS()
class DUSTY_API APoleKeyActor : public AInteractable, public ISaveGameInterface
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
		USphereComponent* m_PoleKeyTrigger;

	UPROPERTY(EditAnywhere, meta = (MakeEditWidget = true))
		FVector m_DustyPosition;
	// Sets default values for this actor's properties
	APoleKeyActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	bool Unlocked = false;


	virtual void LoadActor();

	virtual void OnActorSaved_Implementation();

	virtual void Interact() override;

	virtual void UpdateMaterials();

	FLinearColor EmissiveIntensity;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
		UNiagaraComponent* ParticlePlatform;

	virtual void OnFinishedAutoMovement();

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;


	UPROPERTY(EditAnywhere)
	int m_HudButtonId = 4;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Update(float _DeltaTime, FVector2D _Input);

	virtual void OnPoleInserted();
	virtual void OnPoleRemoved();

	virtual void Activate(bool _bActivation) override;
	virtual void Deactivate() override;
};
