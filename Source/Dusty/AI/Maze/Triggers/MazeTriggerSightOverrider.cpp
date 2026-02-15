#include "MazeTriggerSightOverrider.h"

#include "Dusty/AI/Maze/MazeAIController.h"
#include "Dusty/AI/Maze/MazeNPC.h"
#include "Dusty/Character/DustyCharacter.h"
#include "Kismet/GameplayStatics.h"


void AMazeTriggerSightOverrider::BeginPlay()
{
	Super::BeginPlay();
	m_Player = Cast<ADustyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	PrimaryActorTick.bCanEverTick = false;
}

void AMazeTriggerSightOverrider::DoBeginOverlap(AActor* OtherActor)
{
	if (AMazeNPC* NPC = Cast<AMazeNPC>(OtherActor))
	{
		if (AMazeAIController* EntityController = Cast<AMazeAIController>(NPC->GetController()))
		{
			if (m_Player)
			{
				NPC->SetNeedsToSeePlayer(NeedsToSeePlayer);
			}
		}
	}
}