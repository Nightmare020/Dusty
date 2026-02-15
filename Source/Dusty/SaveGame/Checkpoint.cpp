// Fill out your copyright notice in the Description page of Project Settings.

#include "Checkpoint.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/TriggerBox.h"
#include "Components/BoxComponent.h"
#include "../Character/DustyCharacter.h"
#include "../DustyGameInstance.h"
#include "../Utils/DustyUtils.h"
#include "Dusty/AI/Maze/Triggers/BaseActors/MazeActor.h"
#include "Components/ArrowComponent.h"
#include "../Audio/AudioActor.h"
#include "Dusty/SaveGame/DustySaveGame.h"
#include "Dusty/DustyGameInstance.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

ACheckpoint::ACheckpoint()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("Root"));
	// Create box collider component
	CollisionMesh = CreateDefaultSubobject<UBoxComponent>("Collision Mesh");
	CollisionMesh->SetupAttachment(RootComponent);

	m_ClickablePlaceholder = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ClickablePlaceholder"));
	m_ClickablePlaceholder->SetupAttachment(RootComponent);
	m_ClickablePlaceholder->SetHiddenInGame(true);
	m_ClickablePlaceholder->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	m_CheckPointCameraArrowTransform = CreateDefaultSubobject<UArrowComponent>("CameraArrow");
	m_CheckPointCameraArrowTransform->SetupAttachment(RootComponent);

	m_DustyArrowTransform = CreateDefaultSubobject<UArrowComponent>("DustyArrow");
	m_DustyArrowTransform->SetupAttachment(RootComponent);
	m_DustyArrowTransform->SetArrowColor(FLinearColor::Blue);
}

void ACheckpoint::NotifyActorBeginOverlap(AActor* OtherActor)
{
	// if disabled
	if (!m_Enabled)
	{
		return;
	}

	// check if it's dusty
	if (ADustyCharacter* TempPlayer = Cast<ADustyCharacter>(OtherActor))
	{
		m_PlayerCharacter = TempPlayer;
		m_Enabled = false;
		
		// Get HUD Subsystem
		UHUDSubsystem* hudSubsystem = GetGameInstance()->GetSubsystem<UHUDSubsystem>();

		// do save game
		if (m_HasBeingLoaded || m_OverrideHasBeingLoaded)
		{
			DoSaveGame();
		}
	}
}

void ACheckpoint::BeginPlay()
{
	Super::BeginPlay();
	LoadActor();
	if (m_IsInitialTrigger)
	{
		DustyUtils::PrintMessage("Initial Trigger", FColor::Yellow, 6);
		if (!UGameplayStatics::DoesSaveGameExist(SAVE_SLOT_NAME, 0))
		{
			DustyUtils::PrintMessage("Delokos", FColor::Yellow, 6);
			if (ADustyCharacter* DustyCharacter = DustyUtils::GetDustyCharacter())
			{
				DustyCharacter->SetStartCamera(m_CheckPointCameraArrowTransform->GetComponentTransform(), m_CameraTransitionTime);
				DustyUtils::PrintMessage("PO PO PO", FColor::Yellow, 5);
			}
		}
		else
		{

			UDustySaveGame* SaveGameInstance = Cast<UDustySaveGame>(UGameplayStatics::LoadGameFromSlot(SAVE_SLOT_NAME, 0));

			if (SaveGameInstance)
			{
				FVector SpawnLocation = SaveGameInstance->PlayerSavedData.PlayerTransform.GetLocation();

				DustyUtils::PrintMessage("SpawnLocation", SpawnLocation, FColor::Yellow, 6);
				DustyUtils::PrintMessage("m_InitialPositionForDustyInGame", m_InitialPositionForDustyInGame, FColor::Yellow, 6);

				FVector Difference = (SpawnLocation - m_InitialPositionForDustyInGame);
				DustyUtils::PrintMessage("Difference", Difference.Size(), FColor::Yellow, 6);
				if (Difference.IsNearlyZero(0.1))
				{
					if (ADustyCharacter* DustyCharacter = DustyUtils::GetDustyCharacter())
					{
						DustyCharacter->SetStartCamera(m_CheckPointCameraArrowTransform->GetComponentTransform(), m_CameraTransitionTime);
						DustyUtils::PrintMessage("PO PO PO", FColor::Yellow, 5);
					}
				}
			}
		}

	}
}

void ACheckpoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	m_HasBeingLoaded = true;
	SetActorTickEnabled(false);
}

void ACheckpoint::DoSaveGame()
{
	// If so, cast the game instance to the internal Dusty game instance class
	UDustyGameInstance* DustyGameInstance = GetGameInstance<UDustyGameInstance>();

	// If the cast was right, set the game instance location to the checkpoint location and save it
	if (DustyGameInstance)
	{
		DustyGameInstance->SavePlayerData(m_DustyArrowTransform->GetComponentTransform(), m_CheckPointCameraArrowTransform->GetComponentTransform(), DustyUtils::GetDustyCharacter(), m_CameraTransitionTime, audioActor);
		DustyGameInstance->SaveGame();

		if (!DustyUtils::IsHudVisible(GetWorld(), EHudCanvas::SaveGame) && !m_IsInitialTrigger)
		{
			DustyUtils::ShowHud(GetWorld(), EHudCanvas::SaveGame);
		}
	}

	// Show player save message
	//hudSubsystem->ShowSaveGameMessage();

	// Print to screen using above defined method when actor enters trigger volume
	//DustyUtils::PrintMessage("Game Saved");

	// disable all maze actors needed
	for (AMazeActor* MazeActor : m_MazeActorsToDisable)
	{
		if (MazeActor)
		{
			MazeActor->IsActive = false;
		}
	}
	
}

void ACheckpoint::LoadActor()
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
	Ar << m_Enabled;
	
}

void ACheckpoint::OnActorSaved_Implementation()
{
	FActorSaveData ActorData;

	ActorData.ActorName = GetFName();
	ActorData.LevelName = GetLevel()->GetOuter()->GetFName();
	ActorData.Transform = GetActorTransform();
	FMemoryWriter MemWriter(ActorData.ByteData);

	FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
	Ar.ArIsSaveGame = true;
	// Convert binary array back into actor's variables
	Ar << m_Enabled;

	GetGameInstance<UDustyGameInstance>()->AddActorSaveData(ActorData);
}




