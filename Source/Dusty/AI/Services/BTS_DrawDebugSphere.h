#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTS_DrawDebugSphere.generated.h"

UCLASS()
class DUSTY_API UBTS_DrawDebugSphere : public UBTService_BlackboardBase
{
private:
	GENERATED_BODY()

public:
	UBTS_DrawDebugSphere(const FObjectInitializer& ObjectInitializer);
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	UPROPERTY(EditAnywhere)
	float Radius = 100.f;

	UPROPERTY(EditAnywhere)
	float LifeTime = 0.5;
};
