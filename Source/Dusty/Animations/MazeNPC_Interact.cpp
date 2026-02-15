#include "MazeNPC_Interact.h"

#include "Dusty/AI/Maze/MazeNPC.h"
#include "Dusty/InteractiveObject/Light/LightActor.h"

void UMazeNPC_Interact::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	// check references
	if (!m_MazeNPC)
	{
		m_MazeNPC = Cast<AMazeNPC>(MeshComp->GetOwner());
	}
	
	if (m_MazeNPC)
	{
		if (ALightActor* TargetActor = m_MazeNPC->GetTargetActor())
		{
			bool const CurrentLighting = m_MazeNPC->GetLighting();
			
			TargetActor->Activate(CurrentLighting, m_MazeNPC); // use the current NPC lighting value
			
			m_MazeNPC->SetLighting(!CurrentLighting, false); // set the new value

			// set move so that task can finish on BT
			m_MazeNPC->MazeNPCState = MazeLocomotion::Stop;
		}
	}
}
