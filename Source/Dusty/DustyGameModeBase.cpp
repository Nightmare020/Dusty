// Fill out your copyright notice in the Description page of Project Settings.


#include "DustyGameModeBase.h"
#include "DustyGameInstance.h"

void ADustyGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	GetGameInstance<UDustyGameInstance>()->LoadGame();
}
