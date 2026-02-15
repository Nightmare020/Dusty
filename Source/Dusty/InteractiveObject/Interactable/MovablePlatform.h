// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ToggleActor.h"
#include "MovablePlatform.generated.h"

/**
 * 
 */
UCLASS()
class DUSTY_API AMovablePlatform : public AToggleActor
{
	GENERATED_BODY()
private:
	float m_SpeedTValue;
	float m_currentDelayToActivate = 0;

	FVector m_Destination;
public:
	AMovablePlatform();
	virtual void BeginPlay() override;
	virtual void Activate() override;
	virtual void Deactivate() override;
	virtual void Tick(float deltaTime) override;
	UPROPERTY(EditAnywhere)
	bool m_activated = false;

	UPROPERTY(EditAnywhere, Category = "Design | MovablePlatform")
	bool m_AcceleratedMovement = false;
	UPROPERTY(EditAnywhere, Category = "Design | MovablePlatform")
	float DelayToActivate = 0.f;

	FVector initPos;
	
	UPROPERTY(EditAnywhere, meta = (MakeEditWidget = true))
	FVector finalPos;

	UPROPERTY(EditAnywhere, Category = "Design | MovablePlatform")
	float speed = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Design | MovablePlatform")
	float constantSpeed = 400.0f;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* mesh;

	void LoadActor();

	void OnActorSaved_Implementation();

	UPROPERTY(EditAnywhere)
	UAudioComponent* m_audio;

	bool m_soundPlayed = false;

	UPROPERTY(EditAnywhere, DisplayName = "SoundOffset")
	float m_soundOffset = 0.2f;
};
