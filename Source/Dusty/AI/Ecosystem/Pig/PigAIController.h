#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PigAIController.generated.h"

class UCrowdFollowingComponent;

UCLASS()
class DUSTY_API APigAIController : public AAIController
{
public:
	APigAIController(const FObjectInitializer& ObjectInitializer);

private:
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "AI", DisplayName = "BehaviorTree")
	UBehaviorTree* m_BehaviorTree = nullptr;
};
