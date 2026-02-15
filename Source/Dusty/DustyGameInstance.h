#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SaveGame/DustySaveGame.h"
#include "DustyGameInstance.generated.h"

class UAnimationCurves;
class UAnimationCurvesSubsystem;
class UCinematicsDataAsset;
class UCinematicsSubsystem;
class UHUDSubsystem;
class UMainMenuWidget;
class ADustyCharacter;
class ASittingRock;


UCLASS()
class DUSTY_API UDustyGameInstance : public UGameInstance
{
	GENERATED_BODY()

private:
	TQueue<FName> m_LevelsToLoadQueue;
	UPROPERTY(EditAnywhere)
	TArray<FName> m_LevelsToUnloadQueue;
	TArray<FName> AAAAAAAAAAAAAAAAAA;

	//Si alguno se atreve a criticar esto, que venga y le doy un abracito muy muy fuerte
	FVector Cube1 = FVector(-22.756843, -39036.114020, 735.655326);
	FVector Cube2 = FVector(1019.082588, -35811.671004, 1295.851601);

	void RestartWorld();
	UFUNCTION()
	void UnloadLevelsFromQueue();

	FVector TPDUSTY = FVector(6091.645815f, -38911.589302f, 574.703354f);
	FVector ROTATORDUSTY = FVector(0.f,0.f,169.119867f);
	FVector TPCAMARA = FVector(6733.096927f,-39422.668270f, 1062.255970f);
	FVector ROTATORCAMARA = FVector(-10.600000f, 246.800002f, 0.f);

public:
	UDustyGameInstance();
	UFUNCTION()
	void LoadLevelsFromQueue();

	void LoadStartingLevelsAgain();
	void SetSettingsSaveData(FSettingsSaveData _SettingsSaveData) { SettingsSaveData = _SettingsSaveData; }
	void SaveSettings();

	FSettingsSaveData SettingsSaveData;

	/* Data we need to save or load when we exit or enter the game */
	FVector CurrentPlayerLocation;
	FTransform CameraCheckpoint;

	// New Game button pressed variable
	bool bNewGamePressed = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient, Category = "Animation")
		UAnimationCurves* AnimationCurves;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		TSubclassOf<UAnimationCurvesSubsystem> AnimatonSubsytemRef;

	/** Cinematic Data Asset*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient, Category = "Cinematics")
		UCinematicsDataAsset* CinematicDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cinematics")
		TSubclassOf<UCinematicsSubsystem> CinematicsSubsytemRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hud")
		TSubclassOf<UHUDSubsystem> HudSubsytemRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		FVector PlayerStartLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		FTransform InitialCameraTransform;
	
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient, Category = "New Game")
		//ASittingRock* StartingRockToSit = nullptr;

	TArray<FActorSaveData> SavedActorsFromOtherLevel;
	TArray<FActorSaveData> ActualActorsToSave;
	TMap<FName, TArray<FActorSaveData>> SavedActorsData;

	UFUNCTION(BlueprintCallable)
		void SavePlayerData(FTransform PlayerTransform, FTransform CameraTransform, ADustyCharacter* DustyCharacter, float CameraTransitionTime, FString audioName);

	UFUNCTION(BlueprintCallable)
		void LoadPlayerData(ADustyCharacter* DustyCharacter);

	FActorSaveData GetActorSaveData(FName _ActorName, FName _LevelName);

	/** Saves all actors for all loaded levels */
	UFUNCTION(BlueprintCallable)
	void SaveGame();
	
	/** Loads the game. Loads the player and all the levels that should be loaded based on player position. 
		Then loads the actors of those levels*/
	UFUNCTION(BlueprintCallable)
	void LoadGame();

	/** Saves actors from specified level */
	UFUNCTION(BlueprintCallable)
	void SaveLevel(FName _LevelName, UDustySaveGame* _SaveGameInstance);

	void AddActorSaveData(FActorSaveData _ActorSaveData);

	void LoadLevels();

	void LoadLevel(FName _LevelName);

	void StartNewGame();	
};
