// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelStreamingTrigger.h"
#include "Kismet/GameplayStatics.h"
#include "../Character/DustyCharacter.h"
#include "Components/BoxComponent.h"
#include "../DustyGameInstance.h"
#include "DustySaveGame.h"
#include "SaveGameInterface.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "EngineUtils.h"
#include "../Utils/DustyUtils.h"
#include "Dusty/Utils/DustyConst.h"

// Sets default values
ALevelStreamingTrigger::ALevelStreamingTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("Root")));
	m_Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	m_Trigger->SetupAttachment(RootComponent);

	m_ClickablePlaceholder = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ClickablePlaceholder"));
	m_ClickablePlaceholder->SetupAttachment(RootComponent);
	m_ClickablePlaceholder->SetHiddenInGame(true);
	m_ClickablePlaceholder->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void ALevelStreamingTrigger::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (OtherActor->IsA(ADustyCharacter::StaticClass()))
	{
		if (UGameplayStatics::DoesSaveGameExist(SAVE_SLOT_NAME, 0))
		{
			m_SaveGameInstance = Cast<UDustySaveGame>(UGameplayStatics::LoadGameFromSlot(SAVE_SLOT_NAME, 0));
		}
		else
		{
			m_SaveGameInstance = Cast<UDustySaveGame>(UGameplayStatics::CreateSaveGameObject(UDustySaveGame::StaticClass()));
		}

		FLatentActionInfo LoadLatentInfo;
		for (FName LevelName : LevelsToLoad)
		{
			if (!UGameplayStatics::GetStreamingLevel(this, LevelName)->IsLevelLoaded())
			{				
				LevelsToLoadQueue.Enqueue(LevelName);
			}
		}

		LoadActorsFromLevel();

		FLatentActionInfo UnloadLatentInfo;

		for (FName LevelName : LevelsToUnload)
		{
			if (UGameplayStatics::GetStreamingLevel(this, LevelName)->IsLevelLoaded())
			{
				LevelsToUnloadQueue.Enqueue(LevelName);				
			}
		}
		UnloadActorsFromLevel();
	}
}

void ALevelStreamingTrigger::LoadActorsFromLevel()
{
	if(LevelsToLoadQueue.IsEmpty()) return;	
	FName LevelName;
	LevelsToLoadQueue.Dequeue(LevelName);

	FLatentActionInfo LoadLatentInfo;
	ULevelStreaming* Level = UGameplayStatics::GetStreamingLevel(this, LevelName);
	Level->OnLevelShown.RemoveAll(this);
	Level->OnLevelShown.AddDynamic(this, &ALevelStreamingTrigger::LoadActorsFromLevel);
	UGameplayStatics::LoadStreamLevel(this, LevelName, true, false, LoadLatentInfo);

}

void ALevelStreamingTrigger::UnloadActorsFromLevel()
{

	if (LevelsToUnloadQueue.IsEmpty()) return;
	FName LevelName;
	LevelsToUnloadQueue.Dequeue(LevelName);

	if (UnloadActors)
	{
		GetGameInstance<UDustyGameInstance>()->SaveLevel(LevelName, m_SaveGameInstance);
	}

	ULevelStreaming* Level = UGameplayStatics::GetStreamingLevel(this, LevelName);
	FLatentActionInfo UnloadLatentInfo;
	Level->OnLevelHidden.RemoveAll(this);
	Level->OnLevelHidden.AddDynamic(this, &ALevelStreamingTrigger::UnloadActorsFromLevel);
	UGameplayStatics::UnloadStreamLevel(this, LevelName, UnloadLatentInfo, false);
}


