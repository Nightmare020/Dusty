// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AnimationCurves.generated.h"

class UCurveFloat;

UCLASS()
class DUSTY_API UAnimationCurves : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, DisplayName = "Pole Extension")
		UCurveFloat* PoleExtension;

	UPROPERTY(EditAnywhere, DisplayName = "Pole Hit Extension")
		UCurveFloat* PoleHitExtension;

	UPROPERTY(EditAnywhere, DisplayName = "Jump Smoothness")
		UCurveFloat* Jump;

	UPROPERTY(EditAnywhere, DisplayName = "Pole Extension Limit")
		UCurveFloat* PoleExtensionLimit;

};
