// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlanetsOrbit.h"
#include "PlanetSatelite.generated.h"


UCLASS()
class DUSTY_API APlanetSatelite : public APlanetsOrbit
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
public:
	APlanetSatelite();
	virtual void Tick(float deltaTime) override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* satelite;

	UPROPERTY(EditAnywhere)
	float sateliteSpeed;
	
};
