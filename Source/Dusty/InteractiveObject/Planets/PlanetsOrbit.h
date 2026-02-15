// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dusty/InteractiveObject/Key/Disc.h"
#include "PlanetsOrbit.generated.h"


UCLASS()
class DUSTY_API APlanetsOrbit : public ADisc
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	
public:
	APlanetsOrbit();
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere);
	UStaticMeshComponent* planetMesh;
	int numPlanets = 3;



	UFUNCTION()
	void RotatePlanet();

	void SetOutline(bool Outline);
	
	
};
