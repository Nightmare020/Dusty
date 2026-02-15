// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SaveGameInterface.generated.h"

class UDustySaveGame;

struct FActorSaveData;

UINTERFACE(MinimalAPI)
class USaveGameInterface : public UInterface
{
	GENERATED_BODY()
	
};

class DUSTY_API ISaveGameInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent)
		void OnActorLoaded();

	UFUNCTION(BlueprintNativeEvent)
		void OnActorSaved();
};
