#include "PressurePlateAIController.h"

#include "Dusty/AI/Maze/MazeNPC.h"
#include "Dusty/AI/Maze/Triggers/MazeTriggerTargetOverrider.h"

void APressurePlateAIController::ResolvePuzzleControl()
{
	if(!m_Completed)
	{
		if (m_NPC)
		{
			// set new values
			m_NPC->SetCanMove(true);
			m_NPC->SetNeedsToSeePlayer(false);

			// clear overrides
			m_NPC->OverridedWaypoint = nullptr;
			m_NPC->StoppedBy = nullptr;

			if (m_TriggerToActivate)
			{
				m_TriggerToActivate->IsActive = true;
			}

			// update path
			m_NPC->SetCurrentPathPoint(m_NPC->GetCurrentPathPointIndex()+1);
		}
	}
	
	Super::ResolvePuzzleControl();
}
