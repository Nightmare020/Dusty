// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CinematicsDataAsset.generated.h"

/**
 * 
 */

// Forward declarations
class ULevelSequence;

USTRUCT(BlueprintType)
struct FCinematicInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cinematic")
		ULevelSequence* CurrentSequence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cinematic")
		ULevelSequence* LastShotSequence;
};

UCLASS()
class DUSTY_API UCinematicsDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	/** Map that contains the cinematic level sequence and its tag */
	UPROPERTY(EditAnywhere, DisplayName = "Cinematic")
		TMap<FString, FCinematicInfo> CinematicData;
};
