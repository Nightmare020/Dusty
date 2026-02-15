// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dusty/InteractiveObject/Planets/PlanetsOrbit.h"
#include "Planet.generated.h"

/**
 * 
 */
UCLASS()
class DUSTY_API APlanet : public APlanetsOrbit
{
	GENERATED_BODY()
protected:
	void BeginPlay() override;
	
public:
	APlanet();
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* sateliteMesh;
	UPROPERTY(EditAnywhere)
	USceneComponent* sateliteOrbit;

	UPROPERTY(EditAnywhere)
	float rotationSpeed;

	virtual void Tick(float DeltaSeconds) override;
	virtual void RotateDisc(bool Direction) override;
	
};
