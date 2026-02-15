// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreGravityPlanet.h"
#include "GravitySatelite.h"
#include "../../PhysicsSubsystem.h"

// Sets default values
ACoreGravityPlanet::ACoreGravityPlanet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	mesh = CreateDefaultSubobject<UStaticMeshComponent>("mesh");
	mesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACoreGravityPlanet::BeginPlay()
{
	Super::BeginPlay();

	physicsSubsystem = GetGameInstance()->GetSubsystem<UPhysicsSubsystem>();
	
}

// Called every frame
void ACoreGravityPlanet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	for (AGravitySatelite* satelite : satelites)
	{
		satelite->velocity = (satelite->GetActorLocation() - GetActorLocation());
		float planetDistance = (satelite->GetActorLocation() - GetActorLocation()).Size();

		UE_LOG(LogTemp, Warning, TEXT("PlanetDistance: %f"), (1 / planetDistance) * 2000000);
		physicsSubsystem->SetMagnetForce((1 / planetDistance) * 2000000);
		//satelite->mesh->AddForce((satelite->GetActorLocation() - GetActorLocation()) * -2);
	}
}

