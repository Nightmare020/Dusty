#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTS_GetTadpoleSplineTarget.generated.h"


class ATadpoleAIController;
class ATadpoleNPC;

UCLASS()
class DUSTY_API UBTS_GetTadpoleSplineTarget : public UBTService_BlackboardBase
{
private:
	GENERATED_BODY()
	
private:
	UPROPERTY(Transient)
	ATadpoleAIController* m_EntityController = nullptr;

	UPROPERTY(Transient)
	ATadpoleNPC* m_TadpoleNPC = nullptr;

public:
	UPROPERTY(EditAnywhere)
	float PredictionTime = 0.5f;

public:
	UBTS_GetTadpoleSplineTarget(const FObjectInitializer& ObjectInitializer);
	void virtual OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	void GetReferences (const UBehaviorTreeComponent& OwnerComp);
	
};
