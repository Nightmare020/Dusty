#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTS_CanSeePlayer.generated.h"


class AMazeNPC;
class ADustyCharacter;

UCLASS()
class DUSTY_API UBTS_CanSeePlayer : public UBTService_BlackboardBase
{
private:
	GENERATED_BODY()
	
private:
	UPROPERTY(Transient)
	ADustyCharacter* m_Player = nullptr;
	
	UPROPERTY(Transient)
	AMazeNPC* m_NPC = nullptr;

	UPROPERTY(Transient)
	AAIController* m_EntityController = nullptr;

public:
	UBTS_CanSeePlayer(const FObjectInitializer& ObjectInitializer);
	void virtual OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	UPROPERTY(EditAnywhere, Category = "Design")
	bool CanOverrideMovement = false;

	UPROPERTY(EditAnywhere, Category = "Design")
	float MinDistanceForOverride = 100.f;
	
};
