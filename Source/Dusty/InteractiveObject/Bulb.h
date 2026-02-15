// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bulb.generated.h"

class USphereComponent;

UCLASS()
class DUSTY_API ABulb : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABulb();

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* m_mesh;

	UMaterialInstanceDynamic* m_dynamicMaterial;

	UPROPERTY(EditAnywhere)
	USphereComponent* m_trigger;

	bool m_dustyNear;

	UPROPERTY(EditAnywhere)
	UCurveFloat* m_tentacleCurve;
	
	UPROPERTY(EditAnywhere)
	FAlphaBlend m_tentacleMovement;

	float m_tentacleProgress = 0.0f;
	float m_tentacleBlendTime = 1.0f;

	UPROPERTY(EditAnywhere, DisplayName = "Tentacle Height")
	float m_tentacleHeight = 120.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

};
