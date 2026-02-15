#include "MazeTriggerLighter.h"

#include "Dusty/AI/Maze/MazeNPC.h"

void AMazeTriggerLighter::DoBeginOverlap(AActor* OtherActor)
{
	Super::DoBeginOverlap(OtherActor);
	MazeNPC->SetLighting(m_LightsTarget, false);
	if (m_OneUseOnly)
	{
		IsActive = false;
	}
}
