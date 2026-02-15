// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GravitySatelite.generated.h"

UCLASS()
class DUSTY_API AGravitySatelite : public AActor
{
	GENERATED_BODY()


	UPROPERTY(EditAnywhere)
	float speed;

	
public:	
	// Sets default values for this actor's properties
	AGravitySatelite();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditAnywhere, Transient)
	UStaticMeshComponent* mesh;

	UPROPERTY(EditAnywhere)
	FVector velocity;

};
