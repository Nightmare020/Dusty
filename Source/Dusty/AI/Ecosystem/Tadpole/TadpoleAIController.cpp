#include "TadpoleAIController.h"


void ATadpoleAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATadpoleAIController::BeginPlay()
{
	Super::BeginPlay();
	if (m_BehaviorTree)
	{
		RunBehaviorTree(m_BehaviorTree);
	}
}
