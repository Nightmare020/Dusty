#include "MazeNPC_CanMove.h"

#include "Dusty/AI/Maze/MazeNPC.h"


void UMazeNPC_CanMove::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	// check references
	if (!m_MazeNPC)
	{
		m_MazeNPC = Cast<AMazeNPC>(MeshComp->GetOwner());
	}
	
	if (m_MazeNPC)
	{
		m_MazeNPC->SetCanMove(m_CanMove);
	}
}