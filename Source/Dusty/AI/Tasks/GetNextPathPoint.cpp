// Fill out your copyright notice in the Description page of Project Settings.


#include "GetNextPathPoint.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Dusty/AI/Maze/MazeAIController.h"
#include "Dusty/AI/Maze/MazeNPC.h"
#include "Dusty/AI/Maze/Triggers/MazeTriggerWaypoint.h"

UGetNextPathPoint::UGetNextPathPoint(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Get Next Waypoint");
	bNotifyTick = true;
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UGetNextPathPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (const AMazeAIController* Controller = Cast<AMazeAIController>(OwnerComp.GetAIOwner()))
	{
		if (AMazeNPC* ControllerOwner = Cast<AMazeNPC>(Controller->GetPawn()))
		{
			if (ControllerOwner)
			{
				const AMazeTriggerWaypoint* NextPoint = ControllerOwner->GetNextWaypoint(true);

				if (NextPoint)
				{
					FVector const NewLocation = NextPoint->GetActorLocation();
					OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), NewLocation);
					return EBTNodeResult::Succeeded;
				}
			}
		}
	}

	return EBTNodeResult::Failed;
}