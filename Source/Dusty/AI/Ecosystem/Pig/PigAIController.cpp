#include "PigAIController.h"

#include "Navigation/CrowdFollowingComponent.h"
#include "Navigation/CrowdFollowingComponent.h"

APigAIController::APigAIController(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{
}

void APigAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APigAIController::BeginPlay()
{
	Super::BeginPlay();

	UCrowdFollowingComponent* CrowdFollowingComponent = FindComponentByClass<UCrowdFollowingComponent>();
	if (CrowdFollowingComponent)
	{
		CrowdFollowingComponent->SetCrowdSeparation(true);
		CrowdFollowingComponent->SetCrowdSeparationWeight(50.f);
		CrowdFollowingComponent->SetCrowdAvoidanceRangeMultiplier(1.1f);
	}
	
	if (m_BehaviorTree)
	{
		RunBehaviorTree(m_BehaviorTree);
	}
}
