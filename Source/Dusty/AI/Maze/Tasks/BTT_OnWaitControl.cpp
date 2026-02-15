#include "BTT_OnWaitControl.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Dusty/AI/Maze/MazeNPC.h"
#include "Dusty/Character/DustyCharacter.h"
#include "Dusty/Utils/DustyUtils.h"
#include "Kismet/GameplayStatics.h"


UBTT_OnWaitControl::UBTT_OnWaitControl(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Wait Control");
	bNotifyTick = true;
	bCreateNodeInstance = true;
}


EBTNodeResult::Type UBTT_OnWaitControl::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	m_Player = Cast<ADustyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	m_EntityController = OwnerComp.GetAIOwner();
	if (m_EntityController)
	{
		m_NPC = Cast<AMazeNPC>(m_EntityController->GetPawn());
	}

	if (m_Player && m_NPC)
	{
		m_StartingDistance = DustyUtils::CalculatePathfindingCost(m_NPC->GetActorLocation(), m_Player->GetActorLocation(), GetWorld());
		return EBTNodeResult::InProgress;
	}
	return EBTNodeResult::Failed;
}


void UBTT_OnWaitControl::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (m_TickTimer >= TickTime)
	{
		m_TickTimer = 0.f;
		m_CurrentDistance = DustyUtils::CalculatePathfindingCost(m_NPC->GetActorLocation(), m_Player->GetActorLocation(), GetWorld());

		// get players visibility
		bool const CanSeePlayer = DustyUtils::CanSeePlayer(m_NPC, m_Player, m_NPC->ValidDistance,0.25f);
	
		// if player got close enough and you can either see them or didnt need to see them
		if (m_CurrentDistance < (m_StartingDistance - MaxDistance) && (CanSeePlayer || !NeedsToSeePlayer))
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), false);

			// set can see current value and allow movement again
			//m_NPC->SetCanSee(CanSeePlayer);
			m_NPC->SetCanMove(true);
			
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
		else // check if player is still visible
		{
			// if its too far away and didnt need to see player
			if (FarAwayDistance < m_CurrentDistance && !NeedsToSeePlayer)
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), false);
				m_NPC->SetCanSee(true); // to give some time
				m_NPC->SetCanMove(true);

				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			}
			// if cant see player anymore and needs to see them
			else if(!CanSeePlayer && NeedsToSeePlayer)
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), CanSeePlayer);
				m_NPC->SetCanSee(CanSeePlayer);
				m_NPC->SetCanMove(true);

				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

			}
			// else do nothing
		}
	}
	else
	{
		m_TickTimer += DeltaSeconds;
	}
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}

