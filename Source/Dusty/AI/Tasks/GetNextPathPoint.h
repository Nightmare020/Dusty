#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "GetNextPathPoint.generated.h"


UCLASS()
class DUSTY_API UGetNextPathPoint : public UBTTask_BlackboardBase
{
public:
	UGetNextPathPoint(FObjectInitializer const& ObjectInitializer);
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	GENERATED_BODY()
	
};
