// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DustyGameModeBase.generated.h"

class ASittingRock;

UCLASS()
class DUSTY_API ADustyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient, Category = "New Game")
		ASittingRock* StartingRockToSit = nullptr;

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
};
