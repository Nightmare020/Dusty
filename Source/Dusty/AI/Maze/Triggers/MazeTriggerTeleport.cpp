#include "MazeTriggerTeleport.h"

#include "MazeTriggerWaypoint.h"
#include "NiagaraComponent.h"
#include "Dusty/AI/Maze/MazeNPC.h"


AMazeTriggerTeleport::AMazeTriggerTeleport()
{
	m_Target = CreateDefaultSubobject<USceneComponent>("StartPoint");
	m_Target->SetupAttachment(RootComponent);

	m_Effect = CreateDefaultSubobject<UNiagaraComponent>("Effect");
	m_Effect->SetupAttachment(RootComponent);
}


void AMazeTriggerTeleport::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);
}

void AMazeTriggerTeleport::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (m_TargetActor) // just in case we lose reference
	{
		m_DelayTimer -= DeltaTime;
		if (m_DelayTimer <= 0.f)
		{
			// move actor and activate again visibility and collisions
			m_TargetActor->SetActorHiddenInGame(false);
			m_TargetActor->SetActorLocation(m_TargetLocation);

			// update pathing to next waypoint
			if (m_ControlledWaypoint)
			{
				MazeNPC->SetCurrentPathPoint(m_ControlledWaypoint->MazeId+1);
				m_ControlledWaypoint->SetIsEnabled(true);
			}
			
			// reset actor
			m_TargetActor = nullptr;
			SetActorTickEnabled(false);
		}
	}
}

void AMazeTriggerTeleport::DoBeginOverlap(AActor* OtherActor)
{
	// activate niagara effect
	m_Effect->Activate();
	
	if (m_IsActive) // means is active for teleport procedures
	{
		PrepareTeleport(OtherActor, m_Target);
	}
}

void AMazeTriggerTeleport::DoEndOverlap(AActor* OtherActor)
{
	if (!m_IsActive) // if inactive means needs to be activated and deactivate other
	{
		// m_IsActive = true;
		// if (m_OtherTeleport)
		// {
		// 	m_OtherTeleport->SetIsActive(false);
		// }
	}
}

void AMazeTriggerTeleport::SetIsActive(bool _isActive)
{
	m_IsActive = _isActive;
}

void AMazeTriggerTeleport::PrepareTeleport(AActor* _targetActor, const USceneComponent* _targetScene)
{
	// store actor and target
	m_TargetLocation = _targetScene->GetComponentLocation();
	m_TargetActor = _targetActor;

	// prepare timer
	m_DelayTimer = m_TeleportDelay;
			
	// hidde actor in game
	m_TargetActor->SetActorHiddenInGame(true);
	SetActorTickEnabled(true);
}
