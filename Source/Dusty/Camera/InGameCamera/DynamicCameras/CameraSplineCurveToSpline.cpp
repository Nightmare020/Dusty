#include "CameraSplineCurveToSpline.h"

#include "Camera/CameraComponent.h"
#include "Components/SplineComponent.h"
#include "Dusty/Camera/DustyCameraManager.h"
#include "Dusty/Character/DustyCharacter.h"


ACameraSplineCurveToSpline::ACameraSplineCurveToSpline()
{
	TargetSpline = CreateDefaultSubobject<USplineComponent>("TargetSpline");
	TargetSpline->SetupAttachment(RootComponent);
	
	PlayerSpline = CreateDefaultSubobject<USplineComponent>("PlayerSpline");
	PlayerSpline->SetupAttachment(RootComponent);
}


void ACameraSplineCurveToSpline::SetBlendTargets()
{
	if (m_Mode) // if targeting fixed
	{
		m_TargetLocation = GetCurveSplineClosestPoint();
		FVector const TargetSplineLocation = TargetSpline->FindLocationClosestToWorldLocation(Player->GetActorLocation(), ESplineCoordinateSpace::World);
		FVector const LookAtDirection = TargetSplineLocation - m_TargetLocation;
		m_TargetRotation = FRotationMatrix::MakeFromX(LookAtDirection).Rotator();
	}
	else // if targeting player
	{
		m_TargetLocation = Player->CameraManager->CameraComponent->GetComponentLocation();
		FVector const LookAtDirection = Player->CameraManager->CameraComponent->GetForwardVector();
		m_TargetRotation = FRotationMatrix::MakeFromX(LookAtDirection).Rotator();
	}
}


FVector ACameraSplineCurveToSpline::GetCurveSplineClosestPoint() const 
{
	// get both splines length
	float const PlayerSplineLength = PlayerSpline->GetSplineLength();
	float const TargetSplineLength = CameraSpline->GetSplineLength();

	// get first spline location
	FVector const PlayerSplineLocation = PlayerSpline->FindLocationClosestToWorldLocation(Player->GetActorLocation(), ESplineCoordinateSpace::World);

	// obtain proportional distance on second spline
	float const InputKey = PlayerSpline->FindInputKeyClosestToWorldLocation(PlayerSplineLocation);
	float const PlayerSplineDistance = PlayerSpline->GetDistanceAlongSplineAtSplineInputKey(InputKey);
	float const ProportionalDistanceOnCameraSpline = (PlayerSplineDistance / PlayerSplineLength) * TargetSplineLength;

	// calc target location using proportional distance
	FVector const CameraSplineLocation = CameraSpline->GetLocationAtDistanceAlongSpline(ProportionalDistanceOnCameraSpline, ESplineCoordinateSpace::World);

	return CameraSplineLocation;
}