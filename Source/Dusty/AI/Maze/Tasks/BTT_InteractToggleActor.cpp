#include "BTT_InteractToggleActor.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Dusty/AI/Maze/MazeNPC.h"
#include "Dusty/AI/Maze/Triggers/MazeTriggerBlockMovement.h"
#include "Dusty/Camera/InGameCamera/EventCameras/CameraFocusToInteractingAI.h"
#include "Dusty/InteractiveObject/Light/TimedLightActor.h"
#include "Dusty/Utils/DustyUtils.h"

// this whole logic works in two ways, either there is a camera in which case it has the total control of the process
// or there is not in which case the task will manage the process and the timmings

UBTT_InteractToggleActor::UBTT_InteractToggleActor(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("InteractToggleActor");
	bNotifyTick = true;
	bCreateNodeInstance = true;
}


EBTNodeResult::Type UBTT_InteractToggleActor::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	m_IsOnWait = false;
	if (const AAIController* EntityController = OwnerComp.GetAIOwner())
	{
		m_NPC = Cast<AMazeNPC>(EntityController->GetPawn());
		if (m_NPC)
		{
			m_NPC->SetCurrentPathPoint(m_NPC->GetCurrentPathPointIndex());
			
			m_Timer = 0.f;
			// get initial time for waiting
			if (ALightActor* TargetActor = m_NPC->GetTargetActor())
			{
				// if there is a blocking actor and it has a camera associated the camera takes full control
				if (AMazeTriggerBlockMovement* BlockActor = m_NPC->GetTargetActor()->MazeNPC_TargetActor)
				{
					if(ACameraFocusToInteractingAI* FocusCamera = BlockActor->m_EventCamera)
					{
						// set camera ownership for activation procedures and launch the init
						FocusCamera->SetTriggerOwner(BlockActor);
						FocusCamera->Init();
						
						// so that the task stays blocked till AI is allowed to move again
						m_IsOnWait = true;
						return EBTNodeResult::InProgress;
					}
				}

				// else the task has control but needs to know the time needed to wait
				// if its a timed light actor add its time needed to complete process
				if (const ATimedLightActor* TimedLightActor = Cast<ATimedLightActor>(TargetActor))
				{
					m_Timer += TimedLightActor->Duration + 1.5f; // if anyone sees this 1.5f please just don't judge me
				}
			}
			
			// now that we have the wait time needed call for interacting and start the wait tick process
			m_NPC->MazeNPCState = MazeLocomotion::Interacting; // set mode for anim BP
			return EBTNodeResult::InProgress;
		}
	}
	
	return EBTNodeResult::Failed;
}

void UBTT_InteractToggleActor::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	// if is on wait it means it is only used to block the BT from going on
	if (m_NPC && !m_IsOnWait)
	{
		m_Timer -= DeltaSeconds;
		if (m_Timer <= 0.f) // when timer finishes then it means t
		{
			// interacting has finished
			if (m_NPC->MazeNPCState != MazeLocomotion::Interacting)
			{
				m_NPC->SetCanMove(true);
				m_NPC->SetCurrentState(AIType::Guide);
				
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			}
		}
	}
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}