// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CubeSpawner.generated.h"

class UBoxComponent;
class AMagneticCube;
UCLASS()
class DUSTY_API ACubeSpawner : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Spawner", meta = (AllowPrivateAccess = "true"))
		UBoxComponent* Trigger;


	UPROPERTY(EditAnywhere, Category = "Spawner", meta = (MakeEditWidget = true))
		FVector FirstMagnetismPositon;
	UPROPERTY(EditAnywhere, Category = "Spawner", meta = (MakeEditWidget = true))
		FVector SecondMagnetismPositon;
	UPROPERTY(EditAnywhere, Category = "Spawner", meta = (MakeEditWidget = true))
		bool FirstMagnetismLighted;
	UPROPERTY(EditAnywhere, Category = "Spawner", meta = (MakeEditWidget = true))
		bool SecondMagnetismLighted;

	UPROPERTY(EditAnywhere, Category = "Spawner")
		TSubclassOf<AMagneticCube> MagnetClass;

public:	
	// Sets default values for this actor's properties
	ACubeSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

};
