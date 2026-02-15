#include "TadpoleNPC.h"

#include "TadpoleAIController.h"
#include "TadpoleController.h"
#include "TadpoleSpline.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ATadpoleNPC::ATadpoleNPC()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATadpoleNPC::BeginPlay()
{
	Super::BeginPlay();
}

void ATadpoleNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ATadpoleNPC::SetController(ATadpoleController* _newController)
{
	m_TadpoleController = _newController;
}

void ATadpoleNPC::SetTargetLocation(const FVector& _newTargetLocation)
{
	// set starting target location
	if(m_TadpoleController)
	{
		if(HasAIController())
		{
			m_TadpoleAIController->GetBlackboardComponent()->SetValueAsVector("TargetLocation",_newTargetLocation);
		}
	}
}

FVector ATadpoleNPC::GetNextSplineTarget(float _predictionTime)
{
	if(m_TadpoleController)
	{
		if(m_TargetSpline)
		{
			// get predicted move distance to get new target location
			float TadpoleSpeed = GetCharacterMovement()->GetMaxSpeed();
			float DistanceToMove = _predictionTime * TadpoleSpeed;

			// multiply distance with swim direction
			FVector NewTarget = m_TargetSpline->GetNextLocation(GetActorLocation(), DistanceToMove * SwimDirection);
		
			if (NewTarget == FVector::ZeroVector) // spline finished, swap to another spline
			{
				m_TargetSpline = m_TadpoleController->GetRandomSpline(m_TargetSpline);

				// set new targeting
				return GetTargetSplineClosestLocation();
			}
			return NewTarget;
		}

		// in case there is no spline yet associated
		m_TargetSpline = m_TadpoleController->GetRandomSpline(m_TargetSpline);
		return GetTargetSplineClosestLocation();
	}
	return GetActorLocation(); // just in case
}

FVector ATadpoleNPC::GetTargetSplineClosestLocation()
{
	FVector TargetLocation = GetActorLocation();

	if(m_TargetSpline)
	{
		return m_TargetSpline->GetClosestLocation(TargetLocation, SwimDirection);
	}
	
	return TargetLocation;
}

bool ATadpoleNPC::HasAIController()
{
	if (m_TadpoleAIController)
	{
		return true;
	}
	
	m_TadpoleAIController = Cast<ATadpoleAIController>(GetController());
	return m_TadpoleAIController != nullptr;
}
