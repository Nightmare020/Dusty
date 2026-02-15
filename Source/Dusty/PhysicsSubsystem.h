// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PhysicsSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class DUSTY_API UPhysicsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

private:
	float m_magnetForce = 8000.0f;

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION()
	float GetMagnetForce();

	UFUNCTION()
	void SetMagnetForce(float _magnetForce);

	FVector AddGravity(FVector &_force);
	FVector AddGravity(FVector &_force, float _mass);
	FVector CalcAirDrag(FVector _velocity, float _area);
	FVector CalcFricForce(FVector _velocity, float _mass);
};
