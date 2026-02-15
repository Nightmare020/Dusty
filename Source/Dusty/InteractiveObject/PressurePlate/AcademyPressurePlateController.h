// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dusty/InteractiveObject/PressurePlate/PressurePlateController.h"
#include "AcademyPressurePlateController.generated.h"

class ADissolveActor;

UCLASS()
class DUSTY_API AAcademyPressurePlateController : public APressurePlateController
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "DissolveTime")
		float m_DissolveTime = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "DissolveEntities")
		TArray<ADissolveActor*> DissolveEntities;

public:
	virtual void BeginPlay() override;
	virtual void ResolvePuzzle() override;
};
