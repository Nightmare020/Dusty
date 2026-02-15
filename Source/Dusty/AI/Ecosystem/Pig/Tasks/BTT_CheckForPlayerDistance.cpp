#include "BTT_CheckForPlayerDistance.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Dusty/AI/Ecosystem/Pig/PigNPC.h"
#include "Dusty/Character/DustyCharacter.h"
#include "Kismet/GameplayStatics.h"

UBTT_CheckForPlayerDistance::UBTT_CheckForPlayerDistance()
{
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTT_CheckForPlayerDistance::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (CheckRefs(OwnerComp))
	{
		if(FVector::Dist(m_PigNPC->GetActorLocation(), m_Player->GetActorLocation()) > m_PigNPC->AcceptanceDistance)
		{
			m_PigNPC->SetCurrentState(PigNPCMode::Walking);
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum(GetSelectedBlackboardKey(), static_cast<uint8>(PigNPCMode::Walking));
		}
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}

bool UBTT_CheckForPlayerDistance::CheckRefs(UBehaviorTreeComponent& OwnerComp)
{
	if (m_Player && m_PigNPC)
	{
		return true;
	}
	
	if (const AAIController* EntityController = OwnerComp.GetAIOwner())
	{
		m_PigNPC = Cast<APigNPC>(EntityController->GetPawn());
		m_Player = Cast<ADustyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	}
	return m_PigNPC && m_Player;
}
