// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Audio2DTrigger.generated.h"

class AAudioActor;
class UBoxComponent;
UCLASS()
class DUSTY_API AAudio2DTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAudio2DTrigger();

	UPROPERTY(EditAnywhere);
	UBoxComponent* trigger;

	UPROPERTY(EditAnywhere)
	bool specificAudio = false;

	UPROPERTY(EditAnywhere, DisplayName = "Trigger Parameter")
	FName parameter;
	
	bool activated = false;

	//TEMP for testing
	UFUNCTION(BlueprintNativeEvent)
	void PlayMetasound();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, Category = "Design | Audio Variables", DisplayName = "Audio");
	TArray<AAudioActor*> sounds;
	

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
};
