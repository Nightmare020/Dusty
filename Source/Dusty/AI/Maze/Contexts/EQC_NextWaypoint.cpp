#include "EQC_NextWaypoint.h"

#include "Dusty/AI/Maze/MazeNPC.h"
#include "Dusty/AI/Maze/Triggers/MazeTriggerWaypoint.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"


void UEQC_NextWaypoint::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	Super::ProvideContext(QueryInstance, ContextData);
	
	if (AMazeNPC* OwnerNPC = Cast<AMazeNPC>(QueryInstance.Owner))
	{
		if (AMazeTriggerWaypoint* Waypoint = OwnerNPC->GetNextWaypoint(false))
		{
			if (Waypoint)
			{
				UEnvQueryItemType_Actor::SetContextHelper(ContextData, Cast<AActor>(Waypoint));
			}
		}
	}	
}