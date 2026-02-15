#include "BTS_DrawDebugSphere.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTS_DrawDebugSphere::UBTS_DrawDebugSphere(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "GetChillZone";
	bNotifyBecomeRelevant = true;
	bCreateNodeInstance = true;
	//bNotifyTick = true;
}

void UBTS_DrawDebugSphere::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	FVector SphereLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());

	DrawDebugSphere(GetWorld(), SphereLocation, Radius, 12, FColor::Yellow, false, LifeTime, 0, 2.0f);

}