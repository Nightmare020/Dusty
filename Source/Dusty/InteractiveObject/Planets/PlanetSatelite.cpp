// Fill out your copyright notice in the Description page of Project Settings.


#include "PlanetSatelite.h"

void APlanetSatelite::BeginPlay()
{
	Super::BeginPlay();
}

APlanetSatelite::APlanetSatelite()
{
	satelite = CreateDefaultSubobject<UStaticMeshComponent>("Satelite");
	satelite->SetupAttachment(planetMesh);
}

void APlanetSatelite::Tick(float deltaTime)
{
	FVector newPos = satelite->GetRelativeLocation().RotateAngleAxis(sateliteSpeed, FVector::UpVector);
	satelite->SetRelativeLocation(newPos);
	Super::Tick(deltaTime);

}
