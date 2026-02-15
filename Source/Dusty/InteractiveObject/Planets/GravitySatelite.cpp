// Fill out your copyright notice in the Description page of Project Settings.


#include "GravitySatelite.h"

// Sets default values
AGravitySatelite::AGravitySatelite()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	mesh = CreateDefaultSubobject<UStaticMeshComponent>("mesh");
	mesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AGravitySatelite::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGravitySatelite::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//FVector newPos = FVector((velocity.Y), (-velocity.X), (velocity.Z)) * speed * DeltaTime;
	//DrawDebugPoint(GetWorld(), GetActorLocation(), 1, FColor::Green, true);
	FVector v1 = FVector((velocity.Y), (-velocity.X), (velocity.Z)) * speed * DeltaTime;
	//DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + v1, FColor::Blue, false, 0.2, 0, 2.f);
	//DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() +v1 * 200, FColor::Green, true);
	//DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + FVector(velocity.X, velocity.Y, velocity.Z) * 200, FColor::Yellow, true);

	//FVector v2 = FVector((velocity.X), (velocity.Y), (velocity.Z)) * speed * DeltaTime;
	//DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorUpVector() * 200, FColor::Red, true);

	//FVector cross = FVector::CrossProduct(velocity, GetActorUpVector());
	//DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + cross * 200, FColor::Green, true);

	//FVector cross2 = FVector::CrossProduct(velocity, cross);
	//DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + cross2 * 200, FColor::Yellow, true);
	//v1.Normalize();
	FVector newPos = v1.GetSafeNormal() * speed * DeltaTime;
	SetActorLocation(GetActorLocation() + newPos);
}

