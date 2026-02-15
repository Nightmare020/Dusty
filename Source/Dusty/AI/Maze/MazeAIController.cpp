#include "MazeAIController.h"

#include "MazeNPC.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Triggers/MazeTriggerWaypoint.h"


AMazeAIController::AMazeAIController(FObjectInitializer const& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{
}


void AMazeAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	// set behaviour tree of controller
	ControllerOwner = Cast<AMazeNPC>(InPawn);
	if (ControllerOwner)
	{
		if (UBehaviorTree* const Tree = ControllerOwner->Tree)
		{
			UBlackboardComponent* BlackboardComp;
			UseBlackboard(Tree->BlackboardAsset, BlackboardComp);
			Blackboard = BlackboardComp;
			RunBehaviorTree(Tree);
			
			if (UBehaviorTreeComponent* BehaviorTreeComp = Cast<UBehaviorTreeComponent>(BlackboardComp->GetBrainComponent()))
			{
				BehaviorTreeComp->StopTree(EBTStopMode::Safe);
			}
			
			// initial setters
			// in case there is no starting path point just take the npc location as target since it wont matter
			if (const AMazeTriggerWaypoint* StartingPoint = ControllerOwner->GetNextWaypoint(false))
			{
				BlackboardComp->SetValueAsVector(FName("TargetLocation"), StartingPoint->GetActorLocation());
			}
			else
			{
				BlackboardComp->SetValueAsVector(FName("TargetLocation"), ControllerOwner->GetActorLocation());
			}
			BlackboardComp->SetValueAsBool(FName("CanMove"), true);
			BlackboardComp->SetValueAsBool(FName("CanSeePlayer"), true);
			BlackboardComp->SetValueAsBool(FName("NeedsToSeePlayer"), true);

		}
	}
}

void AMazeAIController::BeginPlay()
{
	Super::BeginPlay();
	
	UCrowdFollowingComponent* CrowdFollowingComponent = FindComponentByClass<UCrowdFollowingComponent>();
	if (CrowdFollowingComponent)
	{
		CrowdFollowingComponent->SetCrowdSeparation(true);
		CrowdFollowingComponent->SetCrowdSeparationWeight(50.f);
		CrowdFollowingComponent->SetCrowdAvoidanceRangeMultiplier(1.1f);
	}
	
}
