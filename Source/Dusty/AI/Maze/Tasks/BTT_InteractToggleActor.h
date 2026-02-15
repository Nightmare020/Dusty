#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_InteractToggleActor.generated.h"


class AMazeNPC;

UCLASS()
class DUSTY_API UBTT_InteractToggleActor : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	explicit UBTT_InteractToggleActor(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	float m_Timer = 0;

	UPROPERTY(Transient)
	AMazeNPC* m_NPC = nullptr;

	bool m_IsOnWait = false;
	
};
