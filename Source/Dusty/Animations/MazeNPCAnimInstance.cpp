#include "MazeNPCAnimInstance.h"

#include "Dusty/AI/Maze/MazeNPC.h"


void UMazeNPCAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	m_MazeNPC = Cast<AMazeNPC>(TryGetPawnOwner());

}

void UMazeNPCAnimInstance::AnimNotify_CheckCanMove()
{
	if(!m_MazeNPC)
	{
		m_MazeNPC = Cast<AMazeNPC>(TryGetPawnOwner());
	}
	
	if (m_MazeNPC)
	{
		m_MazeNPC->CheckCanMove();
	}
}