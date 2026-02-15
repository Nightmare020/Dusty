// Fill out your copyright notice in the Description page of Project Settings.


#include "CinematicTrigger.h"
#include "Components/BoxComponent.h"
#include "../Character/DustyCharacter.h"
#include "../Cinematics/CinematicsSubsystem.h"
#include "../Cinematics/BoxCinematicComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../Cinematics/CinematicsDataAsset.h"
#include "LevelSequence.h"
#include "LevelSequenceActor.h"
#include "MovieScene.h"
#include "../Character/CharacterStates/ActionsFilter.h"
#include "../Utils/DustyUtils.h"
#include "Dusty/SaveGame/DustySaveGame.h"
#include "Dusty/DustyGameInstance.h"
#include "../UI/HUD/PlayerHUDWidget.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

// Sets default values
ACinematicTrigger::ACinematicTrigger()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create box collider component
	CollisionMesh = CreateDefaultSubobject<UBoxCinematicComponent>(FName("Collision Mesh"));
	CollisionMesh->SetupAttachment(RootComponent);
}

void ACinematicTrigger::BeginPlay()
{
	Super::BeginPlay();
	LoadActor();
	SetActorTickEnabled(false);
}

void ACinematicTrigger::NotifyActorBeginOverlap(AActor* OtherActor)
{
	// Check if the overlapping actor is Dusty
	ADustyCharacter* TemporalPlayer = Cast<ADustyCharacter>(OtherActor);

	// Get HUD Subsystem
	UHUDSubsystem* hudSubsystem = GetGameInstance()->GetSubsystem<UHUDSubsystem>();

	// Get HUD Subsystem
	UCinematicsSubsystem* cinematicSubsystem = GetGameInstance()->GetSubsystem<UCinematicsSubsystem>();

	if (TemporalPlayer)
	{
		m_PlayerCharacter = TemporalPlayer;
		SetActorTickEnabled(true);

		if (CollisionMesh->GetCinematicHasPlayed())
		{
			//DustyUtils::PrintMessage("Cinematic already played - Not playing it again");
			return;
		}
		else
		{
			// Show skip cinematic canvas
			hudSubsystem->ShowSkipCinematicMessage();
		}
	}
}

void ACinematicTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (DustyUtils::DustyActuallyInsideTrigger(m_PlayerCharacter, this))
	{		
		if (!CollisionMesh->GetCinematicHasPlayed())
		{
			UCinematicsSubsystem* cinematicSubsystem = GetGameInstance()->GetSubsystem<UCinematicsSubsystem>();

			/** If the cinematic isn't in game, set the player transform to the one stablished by the trigger */
			if (!CollisionMesh->IsInGameCinematic)
			{
				// Set After Cinematic Transform if not in-game cinematic
				cinematicSubsystem->SetTransformNotInGame(CollisionMesh->GetPlayerTransformAfterCinematic());

				// Play Not In Game Cinematic
				cinematicSubsystem->PlayCinematic(CinematicName, CollisionMesh->IsPlayerRelevantInCinematic, CollisionMesh->IsInGameCinematic, CollisionMesh->IsShotsFragmentedCinematic);

				// Evaluate the Skip Cinematic possibility
				CollisionMesh->SkipCinematic();

				// Set Cinemat as Played
				CollisionMesh->SetCinematicPlayed(true);
			}
			else
			{
				if (CinematicName == cinematicSubsystem->ShowFinalQuestionSequence)
				{
					cinematicSubsystem->OnEndCinematic.Clear();
					ADustyCharacter* dusty = DustyUtils::GetDustyCharacter();
					cinematicSubsystem->OnEndCinematic.AddDynamic(dusty, &ADustyCharacter::ShowFinalQuestion);
				}
				// Play In Game Cinematic
				cinematicSubsystem->PlayCinematic(CinematicName);

				// Evaluate the Skip Cinematic possibility
				CollisionMesh->SkipCinematic();

				// Set Cinemat as Played
				CollisionMesh->SetCinematicPlayed(true);
			}
		}
		else
		{
			//DustyUtils::PrintMessage("Cinematic to play is not in the data asset");
		}
				
	}
}

void ACinematicTrigger::LoadActor()
{
	FActorSaveData ActorSaveData = GetGameInstance<UDustyGameInstance>()->GetActorSaveData(GetFName(), GetLevel()->GetOuter()->GetFName());

	if (ActorSaveData.Transform.GetLocation() == FVector::ZeroVector)
	{
		return;
	}
	SetActorTransform(ActorSaveData.Transform);
	FMemoryReader MemReader(ActorSaveData.ByteData);
	FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
	Ar.ArIsSaveGame = true;
	// Convert binary array back into actor's variables
	bool hasPlayed;
	Ar << hasPlayed;
	CollisionMesh->SetCinematicPlayed(hasPlayed);
}

void ACinematicTrigger::OnActorSaved_Implementation()
{
	FActorSaveData ActorData;

	ActorData.ActorName = GetFName();
	ActorData.LevelName = GetLevel()->GetOuter()->GetFName();
	ActorData.Transform = GetActorTransform();
	FMemoryWriter MemWriter(ActorData.ByteData);

	FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
	Ar.ArIsSaveGame = true;
	// Convert binary array back into actor's variables
	bool hasPlayed = CollisionMesh->GetCinematicHasPlayed();
	Ar << hasPlayed;

	GetGameInstance<UDustyGameInstance>()->AddActorSaveData(ActorData);
}