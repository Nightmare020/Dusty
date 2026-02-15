#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_OnWaitControl.generated.h"


class AMazeNPC;
class ADustyCharacter;

UCLASS()
class DUSTY_API UBTT_OnWaitControl : public UBTTask_BlackboardBase
{
private:
	GENERATED_BODY()

private:
	float m_StartingDistance = 0.f;
	float m_CurrentDistance = 0.f;
	float m_TickTimer = 0.f;
	
	UPROPERTY(Transient)
	ADustyCharacter* m_Player = nullptr;
	
	UPROPERTY(Transient)
	AMazeNPC* m_NPC = nullptr;

	UPROPERTY(Transient)
	AAIController* m_EntityController = nullptr;
	
public:
	explicit UBTT_OnWaitControl(FObjectInitializer const& ObjectInitializer);
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	UPROPERTY(EditAnywhere, Category = "Design")
	float MaxDistance = 0.f;

	UPROPERTY(EditAnywhere, Category = "Design")
	float FarAwayDistance = 600.f;

	UPROPERTY(EditAnywhere, Category = "Design")
	float TickTime = 0.f;

	UPROPERTY(EditAnywhere, Category = "Design")
	bool NeedsToSeePlayer = false;
	
};
