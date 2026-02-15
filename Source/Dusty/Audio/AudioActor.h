// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AudioActor.generated.h"

UCLASS()
class DUSTY_API AAudioActor : public AActor
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere)
	UAudioComponent* m_sound;
public:	
	// Sets default values for this actor's properties
	AAudioActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void PlaySound();

	UFUNCTION()
	void StopSound();

	UFUNCTION()
	void PauseSound();

	UFUNCTION()
	void PlayTrigger(const FName& _parameter);

	bool m_isPaused = false;

	
};
