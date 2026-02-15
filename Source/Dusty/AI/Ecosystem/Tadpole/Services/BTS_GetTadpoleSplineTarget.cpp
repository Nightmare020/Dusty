#include "BTS_GetTadpoleSplineTarget.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Dusty/AI/Ecosystem/Tadpole/TadpoleAIController.h"
#include "Dusty/AI/Ecosystem/Tadpole/TadpoleNPC.h"

UBTS_GetTadpoleSplineTarget::UBTS_GetTadpoleSplineTarget(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "GetTargetSplineLocation";
	bNotifyBecomeRelevant = true;
	bCreateNodeInstance = true;
	//bNotifyTick = true;
}

void UBTS_GetTadpoleSplineTarget::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
	GetReferences(OwnerComp);
}

void UBTS_GetTadpoleSplineTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (m_EntityController && m_TadpoleNPC)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), m_TadpoleNPC->GetNextSplineTarget(PredictionTime));
	}
	else
	{
		GetReferences(OwnerComp);
	}
}

void UBTS_GetTadpoleSplineTarget::GetReferences(const UBehaviorTreeComponent& OwnerComp)
{
	m_EntityController = Cast<ATadpoleAIController>(OwnerComp.GetAIOwner());
	if (m_EntityController)
	{
		m_TadpoleNPC = Cast<ATadpoleNPC>(m_EntityController->GetPawn());
	}
}