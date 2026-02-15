#include "PigNPC.h"

#include "PigAIController.h"
#include "PigNPCAreaController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Dusty/Utils/DustyUtils.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Dusty/Audio/AudioSubsytem.h"


APigNPC::APigNPC()
{
	PrimaryActorTick.bCanEverTick = true;
	CurrentSubState = AreaType::Eat;
}

void APigNPC::BeginPlay()
{
	Super::BeginPlay();
	
	SetCurrentState(PigNPCMode::Walking);
	m_PigAIController = Cast<APigAIController>(GetController());

	// im sorry for this
	if (AreasTypesToTarget != PigTargets::Earth && AreasTypesToTarget != PigTargets::Plant)
	{
		if (FMath::RandBool())
		{
			CurrentSubState = AreaType::Eat;
		}
		else
		{
			CurrentSubState = AreaType::Dig;
		}
	}
	else if(AreasTypesToTarget == PigTargets::Fire ||AreasTypesToTarget == PigTargets::Water)
	{
		AreasTypesToTarget = PigTargets::All;
	}
	else if(AreasTypesToTarget == PigTargets::Earth)
	{
		CurrentSubState = AreaType::Dig;
	}
	else if(AreasTypesToTarget == PigTargets::Plant)
	{
		CurrentSubState = AreaType::Eat;
	}
}

void APigNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// Only use timers in chill modes
	if(CurrentState != PigNPCMode::Fleeing && CurrentState != PigNPCMode::Targeting)
	{
		if (!m_MontageCalled)
		{
			m_MontageTimer -= DeltaTime;
			if(m_MontageTimer <= 0.f)
			{
				m_MontageCalled = true;

				if(UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
				{
					AnimInstance->Montage_Play(Montage);
				}
			}
		}

		// control timer for state change
		m_ControlTimer -= DeltaTime;
		if(m_ControlTimer <= 0.f)
		{
			if (CurrentState == PigNPCMode::Walking)
			{
				SetCurrentState(PigNPCMode::Targeting);
			}
			else if (CurrentState == PigNPCMode::Herding || CurrentState == PigNPCMode::Digging)
			{
				SetCurrentState(PigNPCMode::Walking);
			}
		}
	}

	// if(APigAIController* TestController = Cast<APigAIController>(GetController()))
	// {
	// 	FVector TargetLocation = TestController->GetBlackboardComponent()->GetValueAsVector("TargetLocation");
	// 	TargetLocation.Z += 50.f;
	// 	DrawDebugSphere(GetWorld(),TargetLocation, 100.f, 12, FColor::Green, false, -1.0f);
	// }
}

void APigNPC::SetCurrentState(PigNPCMode _newState)
{
	if (_newState != CurrentState)
	{
		// so that it can be used
		m_MontageCalled = false;
		
		// enable tick if it's disabled
		if(!IsActorTickEnabled())
		{
			SetActorTickEnabled(true);
		}
		
		CurrentState = _newState;
		if (CurrentState == PigNPCMode::Walking)
		{
			m_ControlTimer = m_TimeWalking + FMath::FRandRange(0.f, m_RandomVariationTimeWalking);

			// so that montage is not called
			m_MontageCalled = true;
			GetCharacterMovement()->MaxWalkSpeed = m_ChillSpeed;
			//GetCharacterMovement()->MaxAcceleration = m_ChillSpeed;
		}
		else if(CurrentState == PigNPCMode::Targeting)
		{
			// so that montage is not called
			m_MontageCalled = true;
			GetCharacterMovement()->MaxWalkSpeed = m_ChillSpeed;
			GetCharacterMovement()->MaxAcceleration = m_ChillSpeed;

			// flip flop
			if (AreasTypesToTarget == PigTargets::All)
			{
				if (CurrentSubState == AreaType::Dig)
				{
					CurrentSubState = AreaType::Eat;
				}
				else
				{
					CurrentSubState = AreaType::Dig;
				}
			}
			
			// get targeting area
			if (m_AreaController)
			{
				CurrentTarget = m_AreaController->GetClosestArea(this);

				// if there are no available areas then proceed to walking mode again
				if(!CurrentTarget)
				{
					SetCurrentState(PigNPCMode::Walking);
				}
			}
		}
		else if (CurrentState == PigNPCMode::Herding)
		{
			m_ControlTimer = m_TimeHerding + FMath::FRandRange(0.f, m_RandomVariationTimeHerding);
		}
		else if (CurrentState == PigNPCMode::Digging)
		{
			m_ControlTimer = m_TimeDigging + FMath::FRandRange(0.f, m_RandomVariationTimeDigging);
		}
		else
		{
			GetCharacterMovement()->MaxWalkSpeed = m_FleeSpeed;
			GetCharacterMovement()->MaxAcceleration = m_FleeSpeed;

			if (m_sound)
			{
				GetGameInstance()->GetSubsystem<UAudioSubsytem>()->PlaySound2D(m_sound);
			}
		}

		if(m_PigAIController && m_PigAIController->GetBlackboardComponent())
		{
			m_PigAIController->GetBlackboardComponent()->SetValueAsEnum("Mode", static_cast<uint8>(_newState));
		}
		
		m_MontageTimer = m_ControlTimer / 2.f;

		// unlock area if needed
		CleanArea();
	}
}

void APigNPC::CleanArea()
{
	// if it is not targeting an area or consuming it
	if (CurrentState == PigNPCMode::Fleeing || CurrentState == PigNPCMode::Walking)
	{
		if (CurrentTarget)
		{
			CurrentTarget->SetIsFree(true);
			CurrentTarget = nullptr;
		}
	}
}

PigNPCMode APigNPC::GetCurrentState() const
{
	return CurrentState;
}


#pragma region Setters / Getters

float APigNPC::GetDetectionDistance() const
{
	return m_DetectionDistance;
}

void APigNPC::SetDetectionDistance(float _newDetectionDistance)
{
	m_DetectionDistance = _newDetectionDistance;
}

#pragma endregion
