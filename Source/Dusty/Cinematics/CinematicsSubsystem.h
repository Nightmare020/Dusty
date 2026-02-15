// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CinematicsDataAsset.h"
#include "CinematicsSubsystem.generated.h"

// Forward declaration
class ULevelSequence;
class UMovieSceneSequence;
class ADustyCharacter;
class ULevelSequencePlayer;
class ALevelSequenceActor;
class USpringArmComponent;
struct FCinematicInfo;

/**
 * 
 */

// Delegate event for the cinematic beginning
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartCinematic);

// Delegate event for the cinematic ending
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndCinematic);

UCLASS(Abstract, Blueprintable)
class DUSTY_API UCinematicsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	// Begin USubSystem
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// End USubSystem

	/** Flag to check if the cinematic is playing */
	UPROPERTY(BlueprintReadWrite)
		bool IsCinematicPlaying = false;

	/** Name of the player animated character reference in the cinematic */
	UPROPERTY(EditAnywhere, Category = "Cinematics | Cinematic Player")
		FString CinematicPlayerCharacterName = "BP Dusty Character";

	/** Cinematic Data Asset */
	UPROPERTY(EditAnywhere, Transient, BlueprintReadWrite, Category = "Cinematics")
		UCinematicsDataAsset* CinematicDataAsset;

	/** Function to play the normal cinematic from the data asset */
	UFUNCTION(BlueprintCallable, Category = "Cinematics | Cinematic Player")
		void PlayCinematic(FString CinematicName, bool PlayerRelevant = true, bool InGame = true, bool ShotFragmented = false);

	/** Function to play the bench cinematic from the data asset */
	UFUNCTION(BlueprintCallable, Category = "Cinematics | Cinematic Player")
		void PlayBenchCinematic(FString CinematicName, bool PlayerRelevant = true, bool InGame = true, bool ShotFragmented = false);

	/** Function to end the cinematic from the data asset */
	UFUNCTION(BlueprintCallable, Category = "Cinematics | Cinematic Player")
		void EndCinematic();

	/** Get & Set the player cinematic transform after the cinematic if in-game */ 
	FTransform GetTranformNotInGame() { return m_AfterCinematicTransform; }
	void SetTransformNotInGame(FTransform NewTransformAfterCinematic) { m_AfterCinematicTransform = NewTransformAfterCinematic; }

	/** Function to set the cinematic variables for the binding functionality of OnPlay and OnFinished */
	void SetCinematicFlags(bool PlayerRelevant, bool InGame, bool ShotFragmented);

	/** Delegates variables for the start and end of the cinematic */
	FOnStartCinematic OnStartCinematic;
	FOnEndCinematic OnEndCinematic;

	UPROPERTY(EditAnywhere, Category = "Cinematics")
	FString ShowFinalQuestionSequence;
	UPROPERTY(EditAnywhere, Category = "Cinematics")
	FString ShowFinalSequenceNoQuest;
	UPROPERTY(EditAnywhere, Category = "Cinematics")
	FString ShowFinalSequenceQuest;
	UPROPERTY(EditAnywhere, Category = "Cinematics")
	FString CreditsSequence;



private:

	/** Player Character */
	UPROPERTY(Transient)
		ADustyCharacter* m_PlayerCharacter;

	/** Reference to the Level Sequence Actor */
	UPROPERTY(Transient)
		ALevelSequenceActor* m_LevelSequenceActor; 

	/** Reference to the Level Sequence Player */
	UPROPERTY(Transient)
		ULevelSequencePlayer* m_CinematicPlayer;

	/** Struct with the information from the cinematic */
	FCinematicInfo* m_CinematicInfo;

	/** Varaible we'll use for the cinematic info locally */
	UPROPERTY(Transient)
		ULevelSequence* m_CurrentSequence = nullptr;

	/** Player Spring arm */
	UPROPERTY(Transient)
		USpringArmComponent* m_PlayerSpringArm;

	FVector m_OriginalCameraLocation; // Player's Camera original location
	FRotator m_OriginalCameraRotation; // Player's camera original rotation

	/** Current cinematic to play name */
	FString m_CurrentCinematicName;

	/** Cinematic transform after played if the sequence isn't in game */
	FTransform m_PlayerTransformAfterCinematic;

	/** Timer handle to manage the cinematic duration */
	FTimerHandle m_CinematicTimerHandle;

	/** Transform after cinematics if it's not in-game cinematic */
	FTransform m_AfterCinematicTransform;

	/** This function gets the final Dusty position in the cinematic */
	UFUNCTION()
		FTransform GetFinalCharacterTransform(ULevelSequence* Cinematic);

	/** Function that gets the transform of the character in a particular frame */
	UFUNCTION()
		FTransform EvaluateTransformAtFrame(UMovieScene3DTransformSection* TransformSection, FFrameNumber Frame);

	/** Function to get an specific cinematic from the data asset */
	UFUNCTION()
		ULevelSequence* GetCinematic(FString CinematicName);

	/** Function to handle the start of a cinematic */	
	UFUNCTION()
		void BindPlayerToCinematicActor(ULevelSequence* Cinematic);

	/** Variables for the binding cinematic function*/
	bool m_PlayerRelevant;
	bool m_InGame;
	bool m_ShotFragmented;
};
