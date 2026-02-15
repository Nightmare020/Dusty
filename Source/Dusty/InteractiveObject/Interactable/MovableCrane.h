// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MovablePlatform.h"
#include "MovableCrane.generated.h"

/**
 * 
 */
UCLASS()
class DUSTY_API AMovableCrane : public AMovablePlatform
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay();
public:
	AMovableCrane();
	virtual void Tick(float deltaTime);
	virtual void Activate() override;
	virtual void Deactivate() override;
	UPROPERTY(EditAnywhere)
	UChildActorComponent* childActor;
	
};
