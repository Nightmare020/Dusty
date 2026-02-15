#include "MazeTriggerPlayerActivator.h"

#include "MazeTriggerTargetOverrider.h"
#include "MazeTriggerWaypoint.h"

void AMazeTriggerPlayerActivator::SetControlledActorsIsEnabled()
{
	for (AMazeActor* MazeActor: m_MazeActorsToActivate)
	{
		if (MazeActor)
		{
			MazeActor->IsActive = true;
		}
	}
	
	for (AMazeActor* MazeActor: m_MazeActorsToDeactivate)
	{
		if (MazeActor)
		{
			MazeActor->IsActive = false;
		}
	}
}

AMazeTriggerPlayerActivator::AMazeTriggerPlayerActivator()
{
	m_TargetPoint = CreateDefaultSubobject<USceneComponent>("TPLocation");
	m_TargetPoint->SetupAttachment(RootComponent);
}

void AMazeTriggerPlayerActivator::BeginPlay()
{
	Super::BeginPlay();

	if(NeedsToActivatedOnBeginPlay && MazeNPC)
	{
		DoBeginOverlap(MazeNPC);
	}
}

void AMazeTriggerPlayerActivator::DoBeginOverlap(AActor* OtherActor)
{
	Super::DoBeginOverlap(OtherActor);

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
				BehaviorTreeComp->StartTree(*MazeNPC->Tree);
				MazeNPC->SetCurrentPathPoint(0); // first target setted
			}

			if (!m_IsAlreadyOn) // if it has to spawn then spawn
			{
				MazeNPC->SetActorLocation(m_TargetPoint->GetComponentLocation());
				MazeNPC->SetLighting(m_IsLighted, true);
			}

			// set initial values for NPC
			MazeNPC->SetCurrentPathPoint(0);
			MazeNPC->SetCurrentState(m_NewState);
			MazeNPC->SetNeedsToSeePlayer(false);
			MazeNPC->SetCanMove(m_CanMove);

			if (const AMazeTriggerWaypoint* StartingPoint = MazeNPC->GetCurrentPathPoint())
			{
				MazeNPC->SetNPCTargetLocation(StartingPoint->GetActorLocation());
			}

			// disable this trigger
			IsActive = false;
			
			// set actors status
			SetControlledActorsIsEnabled();
		}
	}

	if(!NeedsToActivatedOnBeginPlay)
	{
		NeedsToActivatedOnBeginPlay = true;
		if (m_TriggerActivatorToDisable)
		{
			NeedsToActivatedOnBeginPlay = false;
		}
	}
		
}