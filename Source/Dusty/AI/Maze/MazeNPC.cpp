#include "MazeNPC.h"

#include "MazeAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Dusty/InteractiveObject/Light/LightActor.h"
#include "Dusty/Utils/DustyUtils.h"
#include "Triggers/MazeTriggerBlockMovement.h"
#include "Triggers/MazeTriggerWaypoint.h"


AMazeNPC::AMazeNPC()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMazeNPC::BeginPlay()
{
	Super::BeginPlay();

	m_AIMazeController = Cast<AMazeAIController>(GetController());

	UMaterialInterface* MaterialBase = GetMesh()->GetMaterial(0);

	DynamicMaterial = GetMaterial(MaterialBase);
	GetMesh()->SetMaterial(0, DynamicMaterial);
	 
	SetLighting(m_IsLighted, true);
}

void AMazeNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//DustyUtils::PrintMessage("Current: ", m_CurrentPathPointIndex);
	if(!m_IsCorrectlyLighted)
	{
		UpdateLighting(DeltaTime);
	}
}

void AMazeNPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


// this whole code will be fixed... when design decides :)
AMazeTriggerWaypoint* AMazeNPC::GetNextWaypoint(bool Advance)
{
	if (OverridedWaypoint)
	{
		return OverridedWaypoint;
	}

	if (PathPoints.Num() <= 0)
	{
		return nullptr;
	}
	else
	{
		if (PathPoints.IsValidIndex(m_CurrentPathPointIndex))
		{
			AMazeTriggerWaypoint* Temp = PathPoints[m_CurrentPathPointIndex];
			if (Advance)
			{
				++ m_CurrentPathPointIndex;
			}
			return Temp;
		}
		else if (Advance) // when last waypoint
		{
			if (Controller)
			{
				if (UBehaviorTreeComponent* BehaviorTreeComp = Cast<UBehaviorTreeComponent>(Controller->GetComponentByClass(UBehaviorTreeComponent::StaticClass())))
				{
					BehaviorTreeComp->StopTree(EBTStopMode::Safe);
					++ m_CurrentPathPointIndex;
				}
			}
			return nullptr;
		}
		else
		{
			return PathPoints[PathPoints.Num()-1];
		}
	}
}

#pragma region Getters / Setters

ALightActor* AMazeNPC::GetTargetActor() const
{
	return m_TargetActor;
}

void AMazeNPC::SetTargetActor(ALightActor* _newTarget)
{
	if(m_AIMazeController)
	{
		m_TargetActor = _newTarget;
		m_AIMazeController->GetBlackboardComponent()->SetValueAsVector("TargetLocation", m_TargetActor->MazeNPC_TargetActor->GetActorLocation());
	}

}

void AMazeNPC::SetOverrideWaypoint(AMazeTriggerWaypoint* _overrideWaypoint)
{
	if(m_AIMazeController)
	{
		if(_overrideWaypoint)
		{
			OverridedWaypoint = _overrideWaypoint;
			OverridedWaypoint->SetIsEnabled(true);
			m_AIMazeController->GetBlackboardComponent()->SetValueAsVector("TargetLocation", OverridedWaypoint->GetActorLocation());
		}
		else
		{
			// go to pathing again
			OverridedWaypoint->SetIsEnabled(false);
			OverridedWaypoint = nullptr;
			m_AIMazeController->GetBlackboardComponent()->SetValueAsVector("TargetLocation", PathPoints[m_CurrentPathPointIndex]->GetActorLocation());
		}
	}
}

void AMazeNPC::SetCurrentPathPoint(int _newCurrent, bool _toggle)
{
	if(m_AIMazeController && PathPoints.IsValidIndex(_newCurrent))
	{
		// this is for swap system (Used in Solver AI mainly)
		if (_toggle && PathPoints.IsValidIndex(m_CurrentPathPointIndex))
		{
			PathPoints[m_CurrentPathPointIndex]->SetIsEnabled(false);
			PathPoints[_newCurrent]->SetIsEnabled(true);
		}
	
		m_CurrentPathPointIndex = _newCurrent;
		m_AIMazeController->GetBlackboardComponent()->SetValueAsVector("TargetLocation", PathPoints[m_CurrentPathPointIndex]->GetActorLocation());
	}
}

int AMazeNPC::GetCurrentPathPointIndex() const
{
	return m_CurrentPathPointIndex;
}

AMazeTriggerWaypoint* AMazeNPC::GetCurrentTarget() const
{
	if (OverridedWaypoint)
	{
		return OverridedWaypoint;
	}
	return GetCurrentPathPoint();
}

AMazeTriggerWaypoint* AMazeNPC::GetCurrentPathPoint() const
{
	if (PathPoints.IsValidIndex(m_CurrentPathPointIndex))
	{
		return PathPoints[m_CurrentPathPointIndex];
	}
	return nullptr;
}

void AMazeNPC::SetNeedsToSeePlayer(bool _needsToSeePlayer)
{
	if(m_AIMazeController)
	{
		// testing the override
		m_AIMazeController->GetBlackboardComponent()->SetValueAsBool("NeedsToSeePlayer", false);
	}
}

bool AMazeNPC::GetNeedsToSeePlayer() const
{
	bool CanSeePlayer = false;
	if(m_AIMazeController)
	{
		CanSeePlayer = m_AIMazeController->GetBlackboardComponent()->GetValueAsBool("NeedsToSeePlayer");
	}
	return CanSeePlayer;
}

void AMazeNPC::SetCanMove(bool _canMove)
{
	if(m_AIMazeController)
	{
		if (!_canMove) // if want to stop put it on wait till anim calls it
		{
			m_IsWaitingForStop = true;
		}
		else
		{
			// unless NPC is interacting
			if (MazeNPCState != MazeLocomotion::Interacting)
			{
				m_IsWaitingForStop = false;
				// set locomotion to move so the process starts
				MazeNPCState = MazeLocomotion::Move;
			}
		}
	}
}

bool AMazeNPC::GetCanMove() const
{
	bool CanMove = false;
	if(m_AIMazeController)
	{
		if(!m_IsWaitingForStop) // if it's waiting for stop that means it can't move technically
		{
			CanMove = m_AIMazeController->GetBlackboardComponent()->GetValueAsBool("CanMove");
		}
	}
	return CanMove;
}

void AMazeNPC::CheckCanMove()
{
	if (m_AIMazeController)
	{
		m_AIMazeController->GetBlackboardComponent()->SetValueAsBool("CanMove", true);
		m_AIMazeController->GetBlackboardComponent()->SetValueAsBool("OnWaitControl", false); // just in case
		m_IsWaitingForStop = false;
	}
}

void AMazeNPC::CheckCanStop()
{
	if (m_IsWaitingForStop)
	{
		m_IsWaitingForStop = false; // not waiting anymore
		m_AIMazeController->GetBlackboardComponent()->SetValueAsBool("CanMove", false);
		MazeNPCState = MazeLocomotion::Stop;
	}
}

void AMazeNPC::SetNPCTargetLocation(FVector _targetLocation)
{
	if(m_AIMazeController)
	{
		m_AIMazeController->GetBlackboardComponent()->SetValueAsVector("TargetLocation", _targetLocation);
	}
}
FVector AMazeNPC::GetNPCTargetLocation() const
{
	// use actor as default just in case
	FVector TargetLocation = GetActorLocation();
	if(m_AIMazeController)
	{
		TargetLocation = m_AIMazeController->GetBlackboardComponent()->GetValueAsVector("TargetLocation");
	}
	return TargetLocation;
}

void AMazeNPC::ClearPath()
{
	PathPoints.Empty();
	m_CurrentPathPointIndex = 0;
}

void AMazeNPC::SetCurrentState(AIType _newState)
{
	if(m_AIMazeController)
	{
		m_AIMazeController->GetBlackboardComponent()->SetValueAsEnum("Type", static_cast<uint8>(_newState));
		if (PathPoints.Num() > 0)
		{
			m_AIMazeController->GetBlackboardComponent()->SetValueAsVector("TargetLocation", PathPoints[m_CurrentPathPointIndex]->GetActorLocation());
		}
	}
}

AIType AMazeNPC::GetCurrentState() const
{
	AIType CurrentState = AIType::Guide;
	if(m_AIMazeController)
	{
		CurrentState = static_cast<AIType>(m_AIMazeController->GetBlackboardComponent()->GetValueAsEnum("Type"));
	}
	return CurrentState;
}

void AMazeNPC::SetCurrentLocomotionState(MazeLocomotion _newState)
{
	MazeNPCState = _newState;
}

MazeLocomotion AMazeNPC::GetCurrentLocomotionState() const
{
	return MazeNPCState;
}

void AMazeNPC::SetCanSee(bool _newCanSee)
{
	if(m_AIMazeController)
	{
		m_AIMazeController->GetBlackboardComponent()->SetValueAsBool("CanSeePlayer", _newCanSee);
	}
}

bool AMazeNPC::GetCanSee() const
{
	bool CanSee = false;
	if(m_AIMazeController)
	{
		CanSee = m_AIMazeController->GetBlackboardComponent()->GetValueAsBool("CanSeePlayer");
	}
	return CanSee;
}

bool AMazeNPC::GetLighting() const 
{
	return m_IsLighted;
}

void AMazeNPC::SetLighting(bool _newLighting, bool _IsInstant)
{
	const float TempTarget = (_newLighting) ? LightMaxValue : LightMinValue;

	// set value instantly
	if (_IsInstant)
	{
		DynamicMaterial->SetScalarParameterValue(FName("Position"), TempTarget);
		m_LightValueTarget = TempTarget;
		m_CurrentLightValue = TempTarget;
		m_IsCorrectlyLighted = true;
		m_IsLighted = _newLighting;
		return;
	}
	
	// if already targeting or correctly targeted
	if (m_IsLighted ==_newLighting|| m_LightValueTarget == TempTarget)
	{
		return;
	}	
	
	if (DynamicMaterial)
	{
		// set new target and enable the lerp system
		m_LightValueTarget = TempTarget;
		m_IsCorrectlyLighted = false;
	}
}


void AMazeNPC::PlayMontage(UAnimMontage* AnimMontage)
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->Montage_Play(AnimMontage);
	}
}

void AMazeNPC::UpdateLighting(float DeltaTime)
{
	float const LightDistance = abs(m_LightValueTarget-m_CurrentLightValue);
	float const FAlpha = FMath::Min(DeltaTime * LightBlendSpeed / LightDistance, 1.0f);
	
	m_CurrentLightValue = FMath::Lerp(m_CurrentLightValue, m_LightValueTarget, FAlpha);
	
	DynamicMaterial->SetScalarParameterValue(FName("Position"), m_CurrentLightValue);

	if(m_CurrentLightValue == m_LightValueTarget)
	{
		m_IsCorrectlyLighted = true;

		m_IsLighted = m_LightValueTarget == LightMaxValue;
	}
}

UMaterialInstanceDynamic* AMazeNPC::GetMaterial(UMaterialInterface* _material)
{
	DynamicMaterial = UMaterialInstanceDynamic::Create(_material, this);
	return DynamicMaterial;
}

void AMazeNPC::AddPathPoint(AMazeTriggerWaypoint* _newPathPoint)
{
	PathPoints.Add(_newPathPoint);
}

#pragma endregion
