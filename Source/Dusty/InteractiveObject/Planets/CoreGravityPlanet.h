// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoreGravityPlanet.generated.h"

class AGravitySatelite;
class UPhysicsSubsystem;

UCLASS()
class DUSTY_API ACoreGravityPlanet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACoreGravityPlanet();

	UPROPERTY(EditAnywhere)
	TArray< AGravitySatelite*> satelites;


	UPROPERTY(EditAnywhere, Transient)
	UStaticMeshComponent* mesh;


	UPROPERTY(EditAnywhere, Transient)
		UPhysicsSubsystem* physicsSubsystem;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
