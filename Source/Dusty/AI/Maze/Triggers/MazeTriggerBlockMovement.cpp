#include "MazeTriggerBlockMovement.h"

#include "Dusty/AI/Maze/MazeNPC.h"

void AMazeTriggerBlockMovement::DoBeginOverlap(AActor* OtherActor)
{
	if (Cast<AMazeNPC>(OtherActor))
	{
		MazeNPC->SetCanMove(false);
		IsActive = false;
	}
}
