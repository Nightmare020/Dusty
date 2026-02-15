#include "BTS_IsPlayerTooClose.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Dusty/AI/Ecosystem/Pig/PigAIController.h"
#include "Dusty/AI/Ecosystem/Pig/PigNPC.h"
#include "Dusty/Character/DustyCharacter.h"
#include "Kismet/GameplayStatics.h"

UBTS_IsPlayerTooClose::UBTS_IsPlayerTooClose(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "CheckForPlayer";
	bNotifyBecomeRelevant = true;
	bCreateNodeInstance = true;
	//bNotifyTick = true;
}

void UBTS_IsPlayerTooClose::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
	GetReferences(OwnerComp);
}

void UBTS_IsPlayerTooClose::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (m_EntityController && m_PigNPC && m_Player)
	{
		bool const CanSeePlayer = DustyUtils::CanSeePlayer(m_PigNPC, m_Player, m_PigNPC->GetDetectionDistance() * DetectionFactor,0.25f);
		const PigNPCMode NPCState = m_PigNPC->GetCurrentState();

		if (CanSeePlayer)
		{
			m_CurrentDetectionTimes = MaxDetectionTimes;
			if (NPCState != PigNPCMode::Fleeing) // has detected player and wasn't already fleeing
			{
				m_PigNPC->SetCurrentState(PigNPCMode::Fleeing);
				OwnerComp.GetBlackboardComponent()->SetValueAsEnum(GetSelectedBlackboardKey(), static_cast<uint8>(PigNPCMode::Fleeing));
			}
		}
		else if(NPCState == PigNPCMode::Fleeing)
		{
			m_CurrentDetectionTimes -= 1;
			
			// not detected player enough times
			if(m_CurrentDetectionTimes <= 0)
			{
				m_PigNPC->SetCurrentState(PigNPCMode::Walking);
				OwnerComp.GetBlackboardComponent()->SetValueAsEnum(GetSelectedBlackboardKey(), static_cast<uint8>(PigNPCMode::Walking));
			}
		}
	}
	else
	{
		GetReferences(OwnerComp);
	}
}

void UBTS_IsPlayerTooClose::GetReferences(const UBehaviorTreeComponent& OwnerComp)
{
	m_Player = Cast<ADustyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	m_EntityController = Cast<APigAIController>(OwnerComp.GetAIOwner());
	
	if (m_EntityController)
	{
		m_PigNPC = Cast<APigNPC>(m_EntityController->GetPawn());
	}
}


