#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTS_CheckForPlayer.generated.h"


class ADustyCharacter;

UCLASS()
class DUSTY_API UBTS_CheckForPlayer : public UBTService_BlackboardBase
{
private:
	GENERATED_BODY()

private:
	UPROPERTY(Transient)
	ADustyCharacter* m_Player = nullptr;

public:
	UBTS_CheckForPlayer(const FObjectInitializer& ObjectInitializer);
	void virtual OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
