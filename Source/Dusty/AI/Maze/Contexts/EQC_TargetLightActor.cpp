#include "EQC_TargetLightActor.h"

#include "Dusty/AI/Maze/MazeNPC.h"
#include "Dusty/AI/Maze/Triggers/MazeTriggerBlockMovement.h"
#include "Dusty/InteractiveObject/Light/LightActor.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"

void UEQC_TargetLightActor::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	Super::ProvideContext(QueryInstance, ContextData);

	if (const AMazeNPC* OwnerNPC = Cast<AMazeNPC>(QueryInstance.Owner))
	{
		if (const ALightActor* TargetActor = OwnerNPC->GetTargetActor())
		{
			if (AMazeTriggerBlockMovement* TargetTrigger = TargetActor->MazeNPC_TargetActor)
			{
				UEnvQueryItemType_Actor::SetContextHelper(ContextData, Cast<AActor>(TargetTrigger));
			}
		}
	}	
}
