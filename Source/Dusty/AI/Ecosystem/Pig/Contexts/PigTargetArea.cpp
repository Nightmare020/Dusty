#include "PigTargetArea.h"

#include "Dusty/AI/Ecosystem/Pig/PigNPC.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"


void UPigTargetArea::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	Super::ProvideContext(QueryInstance, ContextData);
	
	if (const APigNPC* OwnerNPC = Cast<APigNPC>(QueryInstance.Owner))
	{
		if (OwnerNPC->CurrentTarget)
		{
			UEnvQueryItemType_Actor::SetContextHelper(ContextData, Cast<AActor>(OwnerNPC->CurrentTarget));
		}
	}	
}