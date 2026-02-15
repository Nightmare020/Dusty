#include "MazeTriggerToggleActor.h"

#include "Dusty/AI/Maze/MazeNPC.h"
#include "Dusty/Camera/InGameCamera/EventCameras/CameraFocusToEvent.h"
#include "Dusty/InteractiveObject/Light/TimedLightActor.h"


AMazeTriggerToggleActor::AMazeTriggerToggleActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMazeTriggerToggleActor::BeginPlay()
{
	Super::BeginPlay();
}

void AMazeTriggerToggleActor::DoBeginOverlap(AActor* OtherActor)
{
	if (m_LightActor && MazeNPC)
	{
		MazeNPC->SetTargetActor(m_LightActor); // set new light actor target
		MazeNPC->SetCurrentState(AIType::Interacting);
		MazeNPC->SetNPCTargetLocation(m_LightActor->GetActorLocation());
		
		SetActorEnableCollision(ECollisionEnabled::NoCollision);
	}
}