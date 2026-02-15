#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTS_IsPlayerTooClose.generated.h"

class ADustyCharacter;
class APigAIController;
class APigNPC;

UCLASS()
class DUSTY_API UBTS_IsPlayerTooClose : public UBTService_BlackboardBase
{
private:
	GENERATED_BODY()
	
private:
	UPROPERTY(Transient)
	ADustyCharacter* m_Player = nullptr;
	
	UPROPERTY(Transient)
	APigAIController* m_EntityController = nullptr;

	UPROPERTY(Transient)
	APigNPC* m_PigNPC = nullptr;
	
	int m_CurrentDetectionTimes = 0;

public:
	UPROPERTY(EditAnywhere)
	int MaxDetectionTimes = 4;

	UPROPERTY(EditAnywhere)
	float DetectionFactor = 1.25f;


public:
	UBTS_IsPlayerTooClose(const FObjectInitializer& ObjectInitializer);
	void virtual OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	void GetReferences (const UBehaviorTreeComponent& OwnerComp);
	
};
