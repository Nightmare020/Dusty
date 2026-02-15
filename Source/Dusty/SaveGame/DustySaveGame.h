// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "DustySaveGame.generated.h"


USTRUCT(BlueprintType)
struct FSettingsSaveData
{
	GENERATED_BODY()
public:
	FSettingsSaveData() {}
	FSettingsSaveData(FString _DisplayMode, FIntPoint _Resolution, int _GeneralVolume, int _MusicVolume, int _EffectsVolume, int _CameraSensitivity, int _AimCameraSensitivity) :
	 DisplayMode(_DisplayMode), Resolution(_Resolution), GeneralVolume(_GeneralVolume), MusicVolume(_MusicVolume), EffectsVolume(_EffectsVolume), CameraSensitivity(_CameraSensitivity), AimCameraSensitivity(_AimCameraSensitivity){}

	UPROPERTY()
		FString DisplayMode = "Windowed Fullscreen";
	UPROPERTY()
		FIntPoint Resolution = FIntPoint(1920, 1080);
	UPROPERTY()
		int GeneralVolume = 100;
	UPROPERTY()
		int MusicVolume = 100;
	UPROPERTY()
		int EffectsVolume = 100;
	UPROPERTY()
		int CameraSensitivity = 100;
	UPROPERTY()
		int AimCameraSensitivity = 100;

};

USTRUCT(BlueprintType)
struct FActorSaveData
{
	GENERATED_BODY()

public:
	/* Identifier for which Actor this belongs to */
	UPROPERTY()
		FName ActorName;
	/* Identifier for which Level this Actor belongs to */
	UPROPERTY()
		FName LevelName;

	/* For movable Actors, keep location,rotation,scale. */
	UPROPERTY()
		FTransform Transform;

	///* Contains all 'SaveGame' marked variables of the Actor */
	UPROPERTY()
		TArray<uint8> ByteData;

	// Declaration of the custom == operator
		bool operator==(const FActorSaveData& Other) const { return (ActorName == Other.ActorName) && (LevelName == Other.LevelName); }

	bool IsValid() const
		{
			// Ensure the ActorName and LevelName are not empty, and ByteData contains something (depending on your game's needs)
			return !ActorName.IsNone() && !LevelName.IsNone() && ByteData.Num() >= 0;
		}
};

USTRUCT()
struct FPlayerSaveData
{
	GENERATED_BODY()

public:
	UPROPERTY()
		FTransform PlayerTransform;

	/* Identifier for which Levels are loaded when Dusty saved game */
	UPROPERTY()
		TArray<FName> LevelNames;

	UPROPERTY()
		FTransform CameraTransform;
	UPROPERTY()
		float CameraTransitionTime = 1;

	UPROPERTY()
		TArray<uint8> ByteData;

	UPROPERTY()
	FString audioName;
};

UCLASS()
class DUSTY_API UDustySaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	//Tiene que ser un UPROPERTY para que lo pueda guardar
	UPROPERTY(VisibleAnywhere)
	FVector PlayerLocation;

	UPROPERTY(VisibleAnywhere)
	FTransform LastCheckpointCameraPos;

	UPROPERTY(EditAnywhere, Category = "Relics")
	TArray<FString> Relics;

	UPROPERTY(VisibleAnywhere)
		TArray<FActorSaveData> SavedActors;
	UPROPERTY(VisibleAnywhere)
		FPlayerSaveData PlayerSavedData;
	UPROPERTY(VisibleAnywhere)
		FSettingsSaveData SettingsSaveData;

};
