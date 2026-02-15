// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevitatingMesh.generated.h"

UCLASS()
class DUSTY_API ALevitatingMesh : public AActor
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* m_Mesh;

	FVector m_InitialPosition;

	UPROPERTY(EditDefaultsOnly, Category = "Art")
		float m_Frequency = 0.5;
	float m_FloatingTimer = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Art")
		float m_Amplitude = 20;
	
public:	
	// Sets default values for this actor's properties
	ALevitatingMesh();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
