#include "MazeNPC_CheckCanMove.h"

#include "Dusty/AI/Maze/MazeNPC.h"

void UMazeNPC_CheckCanMove::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	// check references
	if (!m_MazeNPC)
	{
		m_MazeNPC = Cast<AMazeNPC>(MeshComp->GetOwner());
	}
	
	if (m_MazeNPC)
	{
		if (CheckValue == CheckerValue::CheckMove)
		{
			m_MazeNPC->CheckCanMove();
		}
		else
		{
			m_MazeNPC->CheckCanStop();
		}
	}
}
