#include "CameraFocusToInteractingAI.h"

#include "Dusty/AI/Maze/MazeNPC.h"
#include "Dusty/AI/Maze/Triggers/MazeTriggerBlockMovement.h"

void ACameraFocusToInteractingAI::Reached()
{
	Super::Reached();

	// check for refs
	if (m_Owner && m_Owner->MazeNPC)
	{
		// set the state for the Animation sequence to start
		m_Owner->MazeNPC->MazeNPCState = MazeLocomotion::Interacting;
	}
}

void ACameraFocusToInteractingAI::Exited()
{
	Super::Exited();
	if (m_Owner && m_Owner->MazeNPC)
	{
		// allow move again and set guide mode back
		m_Owner->MazeNPC->SetCanMove(true);
		m_Owner->MazeNPC->SetCurrentState(AIType::Guide);
	}
}

void ACameraFocusToInteractingAI::SetTriggerOwner(AMazeTriggerBlockMovement* _owner)
{
	m_Owner = _owner;

}