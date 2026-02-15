#include "MazeTriggerWaypoint.h"


#include "BehaviorTree/BlackboardComponent.h"
#include "Dusty/AI/Maze/MazeAIController.h"
#include "Dusty/AI/Maze/MazeNPC.h"
#include "Dusty/Utils/DustyUtils.h"
#include "Dusty/Character/DustyCharacter.h"
#include "Dusty/InteractiveObject/Puzzle/DiegoPuzzle/HexagonPuzzlePiece.h"
#include "Kismet/GameplayStatics.h"


AMazeTriggerWaypoint::AMazeTriggerWaypoint()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMazeTriggerWaypoint::BeginPlay()
{
	Super::BeginPlay();
	
	m_player = Cast<ADustyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void AMazeTriggerWaypoint::DoBeginOverlap(AActor* OtherActor)
{
	if (m_isEnabled)
	{
		// just used in AI solver with pillars
		if (m_Pillar)
		{
			m_Pillar->ToggleLighted(true);
		}
		
		if (AMazeNPC* NPC = Cast<AMazeNPC>(OtherActor))
		{
			if (AMazeAIController* EntityController = Cast<AMazeAIController>(NPC->GetController()))
			{
				if (m_player)
				{
					Toggle(NPC, EntityController);
				}
			}
		}
	}
}

void AMazeTriggerWaypoint::DoEndOverlap(AActor* OtherActor)
{
	// literally end overlap only to unlit
	const AMazeNPC* NPC = Cast<AMazeNPC>(OtherActor);
	if (NPC && m_Pillar)
	{
		m_Pillar->ToggleLighted(false);
	}
}

void AMazeTriggerWaypoint::Toggle(AMazeNPC* NPC, AMazeAIController* EntityController)
{
	// block movement and set new waypoint for EQS
	if (NPC->OverridedWaypoint == this)
	{
		NPC->OverridedWaypoint = nullptr;
	}
	else if (NPC->GetCurrentPathPoint() != this)
	{
		return; // if this is not the correct waypoint then exit
	}
				
	// get players visibility
	bool const CanSeePlayer = DustyUtils::CanSeePlayer(NPC, m_player, NPC->ValidDistance,0.25f);

	// set values for control
	NPC->SetCanSee(CanSeePlayer);
	NPC->SetCanMove(!BlocksMobility);

	// tell to the npc who is blocking for backtrack control
	if (BlocksMobility)
	{
		NPC->StoppedBy = this;
	}

	// disable waypoint
	m_isEnabled = false;
}


void AMazeTriggerWaypoint::SetIsEnabled(bool _isEnabled)
{
	m_isEnabled = _isEnabled;
}
bool AMazeTriggerWaypoint::GetIsEnabled() const
{
	return m_isEnabled;
}