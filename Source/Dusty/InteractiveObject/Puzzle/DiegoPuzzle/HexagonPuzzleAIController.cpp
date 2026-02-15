#include "HexagonPuzzleAIController.h"

#include "HexagonPuzzlePiece.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Dusty/AI/Maze/MazeAIController.h"
#include "Dusty/AI/Maze/MazeNPC.h"

void AHexagonPuzzleAIController::BeginPlay()
{
	Super::BeginPlay();

	// Get NPC controller
	if(m_NPC)
	{
		m_AIController = Cast<AMazeAIController>(m_NPC->GetController());
	}
	
	// we need to do a sort algorithm to get the correct value of every level to control the AI
	for (AHexagonPuzzlePiece* Piece: m_Pieces)
	{
		if (Piece->CorrectPillar)
		{
			CorrectPillars.Push(Piece);
		}
	}

	// Sort the TempArray based on the Index property
	Algo::Sort(CorrectPillars, [](const AHexagonPuzzlePiece* A, const AHexagonPuzzlePiece* B)
	{
		return A->Index < B->Index;
	});
}

void AHexagonPuzzleAIController::UpdatePuzzle(APuzzlePiece* TriggerActor)
{
	Super::UpdatePuzzle(TriggerActor);

	if (m_NPC && m_AIController)
	{
		// update the path and allow the NPC to move
		bool IsNewTarget = false;
		bool const IsValidIndex = CorrectPillars.IsValidIndex(m_Index);
		AMazeTriggerWaypoint* CurrentWaypoint = m_NPC->GetCurrentPathPoint();
		
		if(m_CheckPointIndex < m_Index || !IsValidIndex) // once exit point has been reached the AI exits
		{
			m_NPC->SetCurrentPathPoint(m_NumberOfDifferentPillars, true); // get last path point (exit point)
			m_NPC->SetCanMove(true);
			m_NPC = nullptr;
			IsNewTarget = true;
		}
		else if (IsValidIndex) // if is a valid index of pillars then target
		{
			if (m_NPC->GetCurrentPathPointIndex() != CorrectPillars[m_Index]->PillarType) // only update mobility if it's a new target
			{
				m_NPC->SetCurrentPathPoint(CorrectPillars[m_Index]->PillarType, true);
				IsNewTarget = true;
			}
		}

		if (IsNewTarget && m_NPC)
		{
			m_NPC->SetCanMove(true);
		}
	}
}

void AHexagonPuzzleAIController::SetNumberOfPillars(int _numberOfPillars)
{
	m_NumberOfDifferentPillars = _numberOfPillars;
}
