#include "MazeTriggerTargetOverrider.h"

#include "MazeTriggerWaypoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/BoxComponent.h"
#include "Dusty/AI/Maze/MazeAIController.h"
#include "Dusty/AI/Maze/MazeNPC.h"
#include "Dusty/Character/DustyCharacter.h"


AMazeTriggerTargetOverrider::AMazeTriggerTargetOverrider()
{

}

void AMazeTriggerTargetOverrider::BeginPlay()
{
	Super::BeginPlay();
    SetActorTickEnabled(false);
	
	if (MazeNPC)
	{
		m_EntityController = Cast<AMazeAIController>(MazeNPC->GetController());
	}
}

void AMazeTriggerTargetOverrider::DoBeginOverlap(AActor* OtherActor)
{
	Super::DoBeginOverlap(OtherActor);

	// call for trigger management which will also call for override target
	ManageTriggers();
}

void AMazeTriggerTargetOverrider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMazeTriggerTargetOverrider::OverrideTarget() const
{
	if (MazeNPC && m_EntityController)
    {
		MazeNPC->SetCurrentPathPoint(m_PathUpdate);
		m_ControlledWaypoint->SetIsEnabled(false);
		MazeNPC->SetCanMove(true);
    }
}

// void AMazeTriggerTargetOverrider::ManagePathPoints()
// {
// 	if(m_UpdatesPath)
// 	{
// 		if (AMazeTriggerWaypoint* CurrentPathPoint = MazeNPC->GetCurrentPathPoint())
// 		{
// 			MazeNPC->SetCurrentPathPoint(m_PathUpdate);
// 			CurrentPathPoint->SetIsEnabled(true); // enable this again so it can collide
// 		}
// 	}
//
// 	// get target to cover for delay
// 	if (const AMazeTriggerWaypoint* CurrentWaypoint = MazeNPC->GetCurrentTarget())
// 	{
// 		MazeNPC->SetNPCTargetLocation(CurrentWaypoint->GetActorLocation());
// 	}
// }

void AMazeTriggerTargetOverrider::ManageTriggers()
{
	if (MazeNPC && MazeNPC->GetCurrentState() != AIType::PuzzleSolver)
	{
		if (m_OtherTrigger) // if other trigger exists
		{
			// activate referred trigger
			m_OtherTrigger->IsActive = true;
		}
		this->IsActive = false;

		if (m_ControlledWaypoint)
		{
			m_PathUpdate = m_ControlledWaypoint->MazeId + 1;
			OverrideTarget();
		}
	}
}

// void AMazeTriggerTargetOverrider::SetCollision(ECollisionEnabled::Type EnterCollision)
// {
// 	m_Trigger->SetCollisionEnabled(EnterCollision);
// }
