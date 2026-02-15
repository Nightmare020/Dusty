#include "DissolveActorAIController.h"

#include "Dusty/AI/Maze/MazeNPC.h"

#include "Dusty/SaveGame/DustySaveGame.h"
#include "Dusty/DustyGameInstance.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

void ADissolveActorAIController::Activate(bool _bActivation)
{
	if (!HasBeenUsed)
	{
		HasBeenUsed = true;
		Super::Activate(_bActivation);
		m_NPCActivationTimer = Duration;
		SetActorTickEnabled(true);
	}
}

void ADissolveActorAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ADissolveActorAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(m_NPC)
	{
		m_NPCActivationTimer -= DeltaTime;
		if (m_NPCActivationTimer <= 0.f)
		{
			// update path to next waypoint and allow npc to move again
			m_NPC->SetCurrentPathPoint(m_NPC->GetCurrentPathPointIndex()+1);
			m_NPC->SetCanMove(true);
			m_NPC->SetNeedsToSeePlayer(false);
			SetActorTickEnabled(false);
		}
	}
}

void ADissolveActorAIController::LoadActor()
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

	Ar << Lighted;
	Ar << HasBeenUsed;
	
}

void ADissolveActorAIController::OnActorSaved_Implementation()
{
	FActorSaveData ActorData;

	ActorData.ActorName = GetFName();
	ActorData.LevelName = GetLevel()->GetOuter()->GetFName();
	ActorData.Transform = GetActorTransform();

	FMemoryWriter MemWriter(ActorData.ByteData);

	FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
	Ar.ArIsSaveGame = true;

	Ar << Lighted;
	Ar << HasBeenUsed;

	GetGameInstance<UDustyGameInstance>()->AddActorSaveData(ActorData);
}
