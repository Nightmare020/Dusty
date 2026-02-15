// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChangeSkybox.generated.h"

class ASkyAtmosphere;
class UBoxComponent;

UCLASS()
class DUSTY_API AChangeSkybox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChangeSkybox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	UBoxComponent* trigger;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere)
	ASkyAtmosphere* SkyAtmosphereComponent;
	UPROPERTY(EditAnywhere)
	FLinearColor skyScattering;


	bool modifiyingSkybox = false;

	float lerpDuration = 10;
	float elpasedTime = 0;
};
