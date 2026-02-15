#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_CheckForPlayerDistance.generated.h"


class ADustyCharacter;
class APigNPC;

UCLASS()
class DUSTY_API UBTT_CheckForPlayerDistance : public UBTTask_BlackboardBase
{
public:
	UBTT_CheckForPlayerDistance();

private:
	GENERATED_BODY()

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	bool CheckRefs(UBehaviorTreeComponent& OwnerComp);
	
private:
	UPROPERTY(Transient)
	APigNPC* m_PigNPC = nullptr;
	
	UPROPERTY(Transient)
	ADustyCharacter* m_Player = nullptr;
	
};
