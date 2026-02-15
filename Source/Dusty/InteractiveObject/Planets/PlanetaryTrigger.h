// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlanetaryTrigger.generated.h"

class APlanetsKeyController;
class UBoxComponent;

UCLASS()
class DUSTY_API APlanetaryTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlanetaryTrigger();

	UPROPERTY(EditAnywhere)
	UBoxComponent* m_trigger;

	UPROPERTY(EditAnywhere)
	APlanetsKeyController* planetController;

	bool m_active = true;

	UPROPERTY(EditAnywhere, Category = "Design")
	bool m_ElevatorDown = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void RotatePlanets(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
