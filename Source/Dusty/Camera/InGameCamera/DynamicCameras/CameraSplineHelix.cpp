#include "CameraSplineHelix.h"

#include "Camera/CameraComponent.h"
#include "Components/SplineComponent.h"
#include "Dusty/Camera/DustyCameraManager.h"
#include "Dusty/Character/DustyCharacter.h"
#include "Dusty/Character/DustyMovementComponent.h"


ACameraSplineHelix::ACameraSplineHelix()
{
	PlayerSpline = CreateDefaultSubobject<USplineComponent>("PlayerSpline");
	PlayerSpline->SetupAttachment(RootComponent);
}

void ACameraSplineHelix::SetBlendTargets()
{
	GetInputVector();
	
	if (m_Mode) // if targeting fixed
	{
		m_TargetLocation = CameraSpline->FindLocationClosestToWorldLocation(Player->GetActorLocation(), ESplineCoordinateSpace::World);;
		FVector const PlayerLocation = Player->GetActorLocation();
		FVector const LookAtDirection = PlayerLocation - m_TargetLocation;
		m_TargetRotation = FRotationMatrix::MakeFromX(LookAtDirection).Rotator();
	}
	else // if targeting player
	{
		m_TargetLocation = CameraManager->CameraComponent->GetComponentLocation();
		FVector const LookAtDirection = CameraManager->CameraComponent->GetForwardVector();
		m_TargetRotation = FRotationMatrix::MakeFromX(LookAtDirection).Rotator();
	}
}

void ACameraSplineHelix::SetBlendParameters()
{
	if (m_Mode) // if targeting spline
	{
		// // set camera speed to max value depending on the current radius of the player
		// float const PlayerRadius = FVector::Dist(GetActorLocation(), m_Player->GetActorLocation());
		// PlayerSpeedFactor = CameraSplineRadius * PlayerRadius;
		PlayerSpeedFactor = 1.f;
	}
	else
	{
		PlayerSpeedFactor = 1.f;
	}
	Super::SetBlendParameters();
	
}

FRotator ACameraSplineHelix::GetInputVector()
{
	if(!m_MaintainInputDirection)
	{
		// // use tangent to determine player direction in the helix spline
		FVector const ActorLocation = Player->GetActorLocation();
		
		FVector const ClosestPoint = PlayerSpline->FindLocationClosestToWorldLocation(ActorLocation, ESplineCoordinateSpace::World);
		
		FVector const CorrectionDirection = (ClosestPoint - ActorLocation).GetSafeNormal();
		
		FVector const InputDirection = (ClosestPoint - GetActorLocation()).GetSafeNormal() * m_InputDirection;

		//if player goes full W then adjust slightly to compensate missing tangents
		if (abs(Player->MovementInput.Y) == 0.f && abs(Player->MovementInput.X) == 1.f)
		{
			FVector const CorrectiveForce = CorrectionDirection * CorrectionStrength;// * m_Player->MovementInput.Y;
			Player->AddMovementInput(CorrectiveForce, true);
		}
		
		return FRotationMatrix::MakeFromX(InputDirection).Rotator();
	}
	else
	{
		return m_StoredControlRotation;
	}
}