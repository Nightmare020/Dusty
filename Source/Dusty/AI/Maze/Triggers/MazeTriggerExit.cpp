#include "MazeTriggerExit.h"

#include "MazeTriggerPlayerActivator.h"
#include "Dusty/AI/Maze/MazeNPC.h"
#include "Dusty/Character/DustyCharacter.h"


AMazeTriggerExit::AMazeTriggerExit()
{
	m_TargetPoint = CreateDefaultSubobject<USceneComponent>("TPLocation");
	m_TargetPoint->SetupAttachment(RootComponent);
}


void AMazeTriggerExit::BeginPlay()
{
	Super::BeginPlay();
}

void AMazeTriggerExit::DoBeginOverlap(AActor* OtherActor)
{
	if (Cast<AMazeNPC>(OtherActor) || Cast<ADustyCharacter>(OtherActor)) // if is player or NPC
	{
		if (MazeNPC && m_TargetPoint)
		{
			MazeNPC->ClearPath(); // erase all path points from NPC
			MazeNPC->OverridedWaypoint = nullptr;
			MazeNPC->SetActorLocation(m_TargetPoint->GetComponentLocation()); // TP NPC to safe location (not mandatory)

			// find the behavior tree and stop it
			if (const AController* Controller = MazeNPC->GetController())
			{
				if (UBehaviorTreeComponent* BehaviorTreeComp = Cast<UBehaviorTreeComponent>(Controller->GetComponentByClass(UBehaviorTreeComponent::StaticClass())))
				{
					MazeNPC->SetCanMove(false);
					BehaviorTreeComp->StopTree(EBTStopMode::Safe);
				}
			}

			// deactivate activators and triggers
			IsActive = false;
			
			if (m_TriggerActivatorToDisable)
			{
				m_TriggerActivatorToDisable->IsActive = false;
			}

			for (AMazeActor* MazeActor: m_MazeActorsToDeactivate)
			{
				if (MazeActor)
				{
					MazeActor->IsActive = false;
				}
			}
		}
	}
}