// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChangeFog.generated.h"

class UBoxComponent;
class AExponentialHeightFog;

UCLASS()
class DUSTY_API AChangeFog : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		UBoxComponent* Trigger;

	UPROPERTY(EditAnywhere, Category = "Art | Exponential Height Fog Component")
		float FogDensity = 0.0288f;
	UPROPERTY(EditAnywhere, Category = "Art | Exponential Height Fog Component")
		float FogHeightFalloff = 0.51984f;

	UPROPERTY(EditAnywhere, Category = "Art | Exponential Height Fog Component | Second Fog Data")
		float SecondFogDensity = 0.f;
	UPROPERTY(EditAnywhere, Category = "Art | Exponential Height Fog Component | Second Fog Data")
		float SecondFogHeightFalloff = 0.2f;
	UPROPERTY(EditAnywhere, Category = "Art | Exponential Height Fog Component | Second Fog Data")
		float SecondFogHeightOffset = 0.f;

	UPROPERTY(EditAnywhere, Category = "Art | Exponential Height Fog Component")
		FLinearColor FogInscatteringColor = FLinearColor(0,0,0,1);
	UPROPERTY(EditAnywhere, Category = "Art | Exponential Height Fog Component")
		float FogMaxOpacity = 1.f;
	UPROPERTY(EditAnywhere, Category = "Art | Exponential Height Fog Component")
		float StartDistance = 0.f;
	UPROPERTY(EditAnywhere, Category = "Art | Exponential Height Fog Component")
		float FogCutoffDistance = 0.f;


	UPROPERTY(EditAnywhere, Category = "Art | Volumetric Fog")
		bool VolumetricFog = true;
	UPROPERTY(EditAnywhere, Category = "Art | Volumetric Fog")
		float ScatteringDistribution = 0.2f;
	UPROPERTY(EditAnywhere, Category = "Art | Volumetric Fog")
		FColor Albedo = FColor(255,255,255,255);
	UPROPERTY(EditAnywhere, Category = "Art | Volumetric Fog")
		FLinearColor Emissive = FLinearColor(0,0,0,1);

	UPROPERTY(EditAnywhere, Category = "Art | Volumetric Fog")
		float ExtinctionScale = 1.f;
	UPROPERTY(EditAnywhere, Category = "Art | Volumetric Fog")
		float ViewDistance = 6000.f;

	UPROPERTY(EditAnywhere, Category = "Art | ChangeTime")
		float ChangeVelocity = 1.f;

	float Timer = 0;
	bool ChangingFog = false;;


	UPROPERTY(EditAnywhere, Category = "Art")
		AExponentialHeightFog* MapExponentialHeightFog;

	void UpdateFogComponent(UExponentialHeightFogComponent* Fog, float DeltaTime, float InterpSpeed);

	virtual void Tick(float DeltaTime) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
public:	
	// Sets default values for this actor's properties
	AChangeFog();


};
