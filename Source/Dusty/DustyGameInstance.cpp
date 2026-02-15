// Fill out your copyright notice in the Description page of Project Settings.


#include "DustyGameInstance.h"
#include "GameFramework/SaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGame/DustySaveGame.h"
#include "Resources/AnimationCurvesSubsystem.h"
#include "Utils/DustyUtils.h"
#include "UI/MainMenu/MainMenuWidget.h"
#include "InteractiveObject/Interactable/Interactable.h"
#include "InteractiveObject/Interactable/Relic.h"
#include "Utils/DustyConst.h"
#include "SaveGame/SaveGameInterface.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "EngineUtils.h"
#include "InteractiveObject/Light/Magnetism/MagneticCube.h"
#include "Character/DustyCharacter.h"
#include "Dusty/Cinematics/CinematicsSubsystem.h"
#include "Components/PoseableMeshComponent.h"
#include "UI/HUD/PlayerHUDWidget.h"
#include "UI/Book/Book3D/BookActorTextures.h"
#include "UI/Book/Book3D/BookPageWidget.h"
#include "Dusty/DustyGameModeBase.h"
#include "Dusty/UI/Book/CollectibleUnlocker.h"


void UDustyGameInstance::RestartWorld()
{
	for (ULevelStreaming* Level : GetWorld()->GetStreamingLevels())
	{
		if (Level->IsLevelLoaded() && !m_LevelsToUnloadQueue.Contains(FName(Level->GetWorldAsset().GetAssetName())))
		{
			AAAAAAAAAAAAAAAAAA.Add(FName(Level->GetWorldAsset().GetAssetName()));
		}
		
	}
	UnloadLevelsFromQueue();



	ADustyCharacter* dusty = DustyUtils::GetDustyCharacter();
	dusty->SetActorLocation(TPDUSTY);
	dusty->SetActorRotation(ROTATORDUSTY.Rotation());
	dusty->UnlockSkills(false);
	dusty->FirstTimeOpeningBook = true;
	dusty->GetBookActor().LB_RestartBookProgress();

	SavedActorsFromOtherLevel.Empty();
	ActualActorsToSave.Empty();
	SavedActorsData.Empty();

	TArray<AActor*> MagneticCubes; 
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMagneticCube::StaticClass(), MagneticCubes);
	for (int i = 0; i < MagneticCubes.Num(); i++)
	{
		if (AMagneticCube* Miau = Cast<AMagneticCube>(MagneticCubes[i]))
		{
			if (Miau->RestartOnRestart1)
			{
				Miau->SetActorLocation(Cube1);
			}
			else if(Miau->RestartOnRestart2)
			{
				Miau->SetActorLocation(Cube2);
			}
		}
	}

	TArray<AActor*> Collectibles;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACollectibleUnlocker::StaticClass(), Collectibles);
	
		for (int i = 0; i < Collectibles.Num(); i++)
		{
			if (ACollectibleUnlocker* Miau = Cast<ACollectibleUnlocker>(Collectibles[i]))
			{
				if (Miau->IsInInitialMap)
				{
					Miau->RestartCollectible();
				}
			}
		}

}

void UDustyGameInstance::UnloadLevelsFromQueue()
{
	if (AAAAAAAAAAAAAAAAAA.IsEmpty()) return;
	FName LevelName;
	LevelName = AAAAAAAAAAAAAAAAAA.Pop();

	ULevelStreaming* Level = UGameplayStatics::GetStreamingLevel(this, LevelName);
	FLatentActionInfo UnloadLatentInfo;
	Level->OnLevelHidden.RemoveAll(this);
	Level->OnLevelHidden.AddDynamic(this, &UDustyGameInstance::UnloadLevelsFromQueue);
	UGameplayStatics::UnloadStreamLevel(this, LevelName, UnloadLatentInfo, false);
}



UDustyGameInstance::UDustyGameInstance()
{
}

void UDustyGameInstance::SavePlayerData(FTransform PlayerTransform, FTransform CameraTransform, ADustyCharacter* DustyCharacter, float CameraTransitionTime, FString audioName)
{
	UDustySaveGame* SaveGameInstance;
	if (UGameplayStatics::DoesSaveGameExist(SAVE_SLOT_NAME, 0))
	{
		SaveGameInstance = Cast<UDustySaveGame>(UGameplayStatics::LoadGameFromSlot(SAVE_SLOT_NAME, 0));
	}
	else
	{
		SaveGameInstance = Cast<UDustySaveGame>(UGameplayStatics::CreateSaveGameObject(UDustySaveGame::StaticClass()));
	}
	FPlayerSaveData PlayerSaveData;
	PlayerSaveData.PlayerTransform = PlayerTransform;
	PlayerSaveData.CameraTransform = CameraTransform;
	PlayerSaveData.CameraTransitionTime = CameraTransitionTime;
	PlayerSaveData.audioName = audioName;

	
	TArray<FName> LevelNames;
	LevelNames.Add(GetWorld()->PersistentLevel->GetOuter()->GetFName());

	for (ULevelStreaming* Level : GetWorld()->GetStreamingLevels())
	{
		if (Level->IsLevelLoaded())
		{
			LevelNames.Add(FName(Level->GetWorldAsset().GetAssetName()));
		}
	}

	PlayerSaveData.LevelNames = LevelNames;

	// Pass the array to fill with data from Actor
	FMemoryWriter MemWriter(PlayerSaveData.ByteData);

	FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
	// Find only variables with UPROPERTY(SaveGame)
	Ar.ArIsSaveGame = true;
	// Converts Actor's SaveGame UPROPERTIES into binary array
	DustyCharacter->Serialize(Ar);

	SaveGameInstance->PlayerSavedData = PlayerSaveData;
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SAVE_SLOT_NAME, 0);

}

void UDustyGameInstance::SaveGame()
{
	UDustySaveGame* SaveGameInstance;
	if (UGameplayStatics::DoesSaveGameExist(SAVE_SLOT_NAME, 0))
	{
		SaveGameInstance = Cast<UDustySaveGame>(UGameplayStatics::LoadGameFromSlot(SAVE_SLOT_NAME, 0));
	}
	else
	{
		SaveGameInstance = Cast<UDustySaveGame>(UGameplayStatics::CreateSaveGameObject(UDustySaveGame::StaticClass()));
	}
	// Clear all actors from any previously loaded save to avoid duplicates
	
	SaveLevel(GetWorld()->PersistentLevel->GetOuter()->GetFName(), SaveGameInstance);
	for (ULevelStreaming* Level : GetWorld()->GetStreamingLevels())
	{
		if (Level->IsLevelLoaded())
		{
			SaveLevel(FName(Level->GetWorldAsset().GetAssetName()), SaveGameInstance);
		}
	}
	SaveGameInstance->SavedActors.Empty();

	// We get back the actors from other levels to the SavedActors array
	for (FActorSaveData ActorData : SavedActorsFromOtherLevel)
	{
		SaveGameInstance->SavedActors.Add(ActorData);
	}

	SaveGameInstance->SettingsSaveData = SettingsSaveData;

	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SAVE_SLOT_NAME, 0);
	SavedActorsFromOtherLevel.Empty();

}

void UDustyGameInstance::LoadGame()
{

	UDustySaveGame* SaveGameInstance = Cast<UDustySaveGame>(UGameplayStatics::LoadGameFromSlot(SAVE_SLOT_NAME, 0));

	//If first time loading the game, there is no save game instance
	if (SaveGameInstance)
	{
		for (const FActorSaveData& ActorData : SaveGameInstance->SavedActors)
		{
			if (SavedActorsData.Find(ActorData.LevelName) == nullptr)
			{
				SavedActorsData.Add(ActorData.LevelName, TArray<FActorSaveData>());
			}
			SavedActorsData[ActorData.LevelName].Add(ActorData);
		}

		SettingsSaveData = SaveGameInstance->SettingsSaveData;
	}
}


void UDustyGameInstance::LoadLevelsFromQueue()
{
	if (m_LevelsToLoadQueue.IsEmpty()) return;
	FName LevelName;
	m_LevelsToLoadQueue.Dequeue(LevelName);

	//DustyUtils::PrintMessage(LevelName.ToString());

	FLatentActionInfo LoadLatentInfo;
	ULevelStreaming* Level = UGameplayStatics::GetStreamingLevel(this, LevelName);
	//Level->OnLevelShown.RemoveAll(this);
	//Level->OnLevelShown.AddDynamic(this, &UDustyGameInstance::LoadLevelsFromQueue);

	FTimerHandle LoadHandle;
	GetWorld()->GetTimerManager().SetTimer(LoadHandle, this, &UDustyGameInstance::LoadLevelsFromQueue, 0.25, false);
	UGameplayStatics::LoadStreamLevel(this, LevelName, true, true, LoadLatentInfo);
}

void UDustyGameInstance::LoadStartingLevelsAgain()
{
	UDustySaveGame* SaveGameInstance = Cast<UDustySaveGame>(UGameplayStatics::LoadGameFromSlot(SAVE_SLOT_NAME, 0));

	if (SaveGameInstance)
	{

		for (int i = 0; i < SaveGameInstance->PlayerSavedData.LevelNames.Num(); i++)
		{
			FName LevelName = SaveGameInstance->PlayerSavedData.LevelNames[i];

			ULevelStreaming* Level = UGameplayStatics::GetStreamingLevel(this, LevelName);
			if (Level && !Level->IsLevelLoaded())
			{
				DustyUtils::PrintMessage(LevelName.ToString());
				m_LevelsToLoadQueue.Enqueue(LevelName);
			}
		}
		LoadLevelsFromQueue();
	} 
}

void UDustyGameInstance::SaveSettings()
{
	UDustySaveGame* SaveGameInstance;
	if (UGameplayStatics::DoesSaveGameExist(SAVE_SLOT_NAME, 0))
	{
		SaveGameInstance = Cast<UDustySaveGame>(UGameplayStatics::LoadGameFromSlot(SAVE_SLOT_NAME, 0));
	}
	else
	{
		SaveGameInstance = Cast<UDustySaveGame>(UGameplayStatics::CreateSaveGameObject(UDustySaveGame::StaticClass())); 
		FPlayerSaveData PlayerSaveData;
		PlayerSaveData.PlayerTransform.SetLocation(TPDUSTY);
		PlayerSaveData.PlayerTransform.SetRotation(ROTATORDUSTY.Rotation().Quaternion());
		PlayerSaveData.CameraTransform.SetLocation(TPCAMARA);
		PlayerSaveData.CameraTransform.SetRotation(ROTATORCAMARA.Rotation().Quaternion());
		SaveGameInstance->PlayerSavedData = PlayerSaveData;
	}	

	SaveGameInstance->SettingsSaveData = SettingsSaveData;
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SAVE_SLOT_NAME, 0);
}

void UDustyGameInstance::SaveLevel(FName _LevelName, UDustySaveGame* _SaveGameInstance)
{

	UE_LOG(LogTemp, Warning, TEXT("SaveLevelStarted"));
	//If first time loading the game, there is no save game instance
	if (_SaveGameInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("_SaveGameInstance exists"));
		// When saving, we have the info of all actors from all levels
		// So we save the ones from other levels to not override them
		if (_SaveGameInstance->SavedActors.Num() != 0.f)
		{
			UE_LOG(LogTemp, Warning, TEXT("SavedActors > 0"));
			for (int i = 0; i < _SaveGameInstance->SavedActors.Num(); i++)
			{
				UE_LOG(LogTemp, Warning, TEXT("Inside for"));
				const FActorSaveData& ActorData = _SaveGameInstance->SavedActors[i];
				
				if (ActorData.LevelName != _LevelName && !SavedActorsFromOtherLevel.Contains(ActorData))
				{
					UE_LOG(LogTemp, Warning, TEXT("SavedActorsFromOtherLevel Add"));
					SavedActorsFromOtherLevel.Add(ActorData);
				}

			}
		}
		
		UE_LOG(LogTemp, Warning, TEXT("Iterate through all actors"));
		// Iterate the entire world of actors
		for (FActorIterator It(GetWorld()); It; ++It)
		{
			AActor* Actor = *It;

			// Only interested in our savable actors and from current level

			if (!Actor->GetClass()->ImplementsInterface(USaveGameInterface::StaticClass()) ||
				Actor->GetLevel()->GetOuter()->GetFName() != _LevelName)
			{
				continue;
			}
			UE_LOG(LogTemp, Warning, TEXT("Actor Is Savable"));

			ISaveGameInterface::Execute_OnActorSaved(Actor);
			
		}


	}
}

void UDustyGameInstance::AddActorSaveData(FActorSaveData _ActorSaveData)
{
	int index = SavedActorsFromOtherLevel.Find(_ActorSaveData);
	//If it already contains info of the actor we want to save, we need to override it
	if (index != INDEX_NONE)
	{
		SavedActorsFromOtherLevel.RemoveAt(index);
	}
	// We add the actor to the SavedActors array of the SaveGameInstance
	SavedActorsFromOtherLevel.Add(_ActorSaveData);
	

	// If we are saving a level for the first time, we need to add it to the SavedActorsData map
		// Because, when loading the level again, the initial map for the initial load doesn't containt	the level
	if (SavedActorsData.Find(_ActorSaveData.LevelName) == nullptr)
	{
		SavedActorsData.Add(_ActorSaveData.LevelName, TArray<FActorSaveData>());
	}

	// If we are saving an actor of the level for the first time, we need to add it to the SavedActorsData array of that level
	// So next time, when loading, the actor can find it's data
	if (SavedActorsData[_ActorSaveData.LevelName].Find(_ActorSaveData) == INDEX_NONE)
	{
		SavedActorsData[_ActorSaveData.LevelName].Add(_ActorSaveData);
	}
	// If we are saving an actor of the level that already exists, we need to update it's data
	else
	{
		for (FActorSaveData& Data : SavedActorsData[_ActorSaveData.LevelName])
		{
			if (Data == _ActorSaveData)
			{
				Data = _ActorSaveData;
			}
		}
	}
	
}

void UDustyGameInstance::LoadLevels()
{
	UDustySaveGame* SaveGameInstance;
	if (UGameplayStatics::DoesSaveGameExist(SAVE_SLOT_NAME, 0))
	{
		SaveGameInstance = Cast<UDustySaveGame>(UGameplayStatics::LoadGameFromSlot(SAVE_SLOT_NAME, 0));
	}
	else
	{
		return;
	}
	TArray<FName> LevelNames = SaveGameInstance->PlayerSavedData.LevelNames;

	for (FName LevelName : LevelNames)
	{
		ULevelStreaming* Level = UGameplayStatics::GetStreamingLevel(this, LevelName);
		if (Level && !Level->IsLevelLoaded())
		{			
			m_LevelsToLoadQueue.Enqueue(LevelName);
		}
	}
	LoadLevelsFromQueue();
}

void UDustyGameInstance::LoadLevel(FName _LevelName)
{
	FLatentActionInfo LoadLatentInfo;
	UGameplayStatics::LoadStreamLevel(this, _LevelName, true, true, LoadLatentInfo);
	
}

void UDustyGameInstance::StartNewGame()
{	
	ADustyCharacter* Dusty = DustyUtils::GetDustyCharacter();
	FVector DustyLocation = Dusty->GetActorLocation();
	float Distance = FVector::Distance(TPDUSTY, DustyLocation);
	DustyUtils::PrintMessage("Distance", Distance);
	if (Distance > 50)
	{
		UGameplayStatics::DeleteGameInSlot(SAVE_SLOT_NAME, 0);

		UKismetSystemLibrary::QuitGame(GetWorld(), NULL, EQuitPreference::Quit, true);
	}
	else
	{
		UCinematicsSubsystem* cinematicsSubsystem = GetSubsystem<UCinematicsSubsystem>();
		cinematicsSubsystem->PlayCinematic("LS_Intro_Master", true, false, true);

		UHUDSubsystem* hudSubsystem = GetSubsystem<UHUDSubsystem>();

		ADustyCharacter* dusty = DustyUtils::GetDustyCharacter();
		cinematicsSubsystem->OnEndCinematic.AddDynamic(dusty, &ADustyCharacter::NewGameCinematicEnd);
		//Cast<ADustyGameModeBase>(GetWorld()->GetAuthGameMode())->StartingRockToSit->SitDusty(dusty);

		// Show canvas
		hudSubsystem->ShowSkipCinematicMessage();

		//In case we cant properly bind
		dusty->GetMesh()->SetVisibility(false);
		dusty->PoleSkeletal->SetVisibility(false);
	}
	
}

void UDustyGameInstance::LoadPlayerData(ADustyCharacter* DustyCharacter)
{
	UDustySaveGame* SaveGameInstance = Cast<UDustySaveGame>(UGameplayStatics::LoadGameFromSlot(SAVE_SLOT_NAME, 0));
	
	if (SaveGameInstance && DustyCharacter)
	{
		DustyCharacter->SetActorTransform(SaveGameInstance->PlayerSavedData.PlayerTransform);
		DustyCharacter->SetStartCamera(SaveGameInstance->PlayerSavedData.CameraTransform, SaveGameInstance->PlayerSavedData.CameraTransitionTime);
		FMemoryReader MemReader(SaveGameInstance->PlayerSavedData.ByteData);

		for (int i = 0; i < SaveGameInstance->PlayerSavedData.LevelNames.Num(); i++)
		{
			FName LevelName = SaveGameInstance->PlayerSavedData.LevelNames[i];

			ULevelStreaming* Level = UGameplayStatics::GetStreamingLevel(this, LevelName);
			if (Level && !Level->IsLevelLoaded())
			{
				m_LevelsToLoadQueue.Enqueue(LevelName);
			}
		}
		LoadLevelsFromQueue();

		FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
		Ar.ArIsSaveGame = true;
		// Convert binary array back into actor's variables
		DustyCharacter->Serialize(Ar);
		DustyCharacter->OnActorLoaded();
	}	
}

FActorSaveData UDustyGameInstance::GetActorSaveData(FName _ActorName, FName _LevelName)
{
	if (SavedActorsData.Find(_LevelName) == nullptr)
	{
		return FActorSaveData();
	}
	for (FActorSaveData ActorData : SavedActorsData[_LevelName])
	{
		if (ActorData.ActorName == _ActorName)
		{
			return ActorData;
		}
	}
	return FActorSaveData();
}
