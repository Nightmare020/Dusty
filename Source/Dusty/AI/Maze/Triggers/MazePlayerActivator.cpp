#include "MazePlayerActivator.h"


#include "MazeTriggerTargetOverrider.h"
#include "MazeTriggerWaypoint.h"
#include "Dusty/AI/Maze/MazeNPC.h"


AMazePlayerActivator::AMazePlayerActivator()
{
	m_TargetPoint = CreateDefaultSubobject<USceneComponent>("TPLocation");
	m_TargetPoint->SetupAttachment(RootComponent);
	
	PrimaryActorTick.bCanEverTick = false;
}

void AMazePlayerActivator::BeginPlay()
{
	Super::BeginPlay();
}

void AMazePlayerActivator::Activate()
{
	if (m_TargetPoint && MazeNPC)
	{
		if (AController* Controller = MazeNPC->GetController())
		{
			if (m_IsAlreadyOn) // in case NPC has followed player to a new event just clean and dont TP
			{
				MazeNPC->ClearPath();
			}
			
			if (UBehaviorTreeComponent* BehaviorTreeComp = Cast<UBehaviorTreeComponent>(Controller->GetComponentByClass(UBehaviorTreeComponent::StaticClass())))
			{
				int MazeId = 0;
				// add all waypoints to NPC path point TArray
				for (AMazeTriggerWaypoint* Waypoint : PathPoints)
				{
					if (Waypoint)
					{
						MazeNPC->AddPathPoint(Waypoint);

						// set ID for overriding control
						Waypoint->MazeId = MazeId;
						++MazeId;
					}
				}

				SetActorEnableCollision(ECollisionEnabled::NoCollision);
				BehaviorTreeComp->StartTree(*MazeNPC->Tree);
			}

			if (!m_IsAlreadyOn) // if it has to spawn then spawn
			{
				MazeNPC->SetActorLocation(m_TargetPoint->GetComponentLocation());
			}

			// set initial values for NPC
			MazeNPC->SetCurrentState(m_NewState);
			MazeNPC->SetNeedsToSeePlayer(m_NeedsToSeePlayer);
			MazeNPC->SetCanMove(m_CanMove);

			if (const AMazeTriggerWaypoint* StartingPoint = MazeNPC->GetCurrentPathPoint())
			{
				MazeNPC->SetNPCTargetLocation(StartingPoint->GetActorLocation());
			}
		}
	}
}