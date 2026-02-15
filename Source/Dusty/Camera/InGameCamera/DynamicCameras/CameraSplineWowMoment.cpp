#include "CameraSplineWowMoment.h"

#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/SplineComponent.h"
#include "Dusty/Camera/DustyCameraManager.h"
#include "Dusty/Character/DustyCharacter.h"


ACameraSplineWowMoment::ACameraSplineWowMoment()
{
	TargetSpline = CreateDefaultSubobject<USplineComponent>("TargetSpline");
	TargetSpline->SetupAttachment(RootComponent);
	
	PlayerSpline = CreateDefaultSubobject<USplineComponent>("PlayerSpline");
	PlayerSpline->SetupAttachment(RootComponent);
	
	InputDirection = CreateDefaultSubobject<UArrowComponent>("InputDirection");
	InputDirection->SetupAttachment(RootComponent);
}

void ACameraSplineWowMoment::BeginPlay()
{
	Super::BeginPlay();
	PlayerSplineLength = PlayerSpline->GetSplineLength();
	CameraSplineLength = CameraSpline->GetSplineLength();
	TargetSplineLength = TargetSpline->GetSplineLength();
}


FRotator ACameraSplineWowMoment::GetInputVector()
{
	if(!m_MaintainInputDirection)
	{
		return InputDirection->GetForwardVector().Rotation();
	}
	else
	{
		return m_StoredControlRotation;
	}
}

void ACameraSplineWowMoment::SetBlendTargets()
{
	if (m_Mode) // if targeting fixed
	{
		FVector const CurrentPlayerSplineLocation = PlayerSpline->FindLocationClosestToWorldLocation(Player->GetActorLocation(), ESplineCoordinateSpace::World);
		float const InputKey = PlayerSpline->FindInputKeyClosestToWorldLocation(CurrentPlayerSplineLocation);
		float const PlayerSplineDistance = PlayerSpline->GetDistanceAlongSplineAtSplineInputKey(InputKey);
		
		m_TargetLocation = GetCurveSplineClosestPoint(PlayerSplineDistance); // camera spline for camera location using relative positioning on players spline
		m_TargetRotation = GetCurrentRotation(PlayerSplineDistance); // target spline for camera rotation using relative positioning on players spline
	}
	else // if targeting player
	{
		m_TargetLocation = Player->CameraManager->CameraComponent->GetComponentLocation();
		FVector const LookAtDirection = Player->CameraManager->CameraComponent->GetForwardVector();
		m_TargetRotation = FRotationMatrix::MakeFromX(LookAtDirection).Rotator();
	}
}


FVector ACameraSplineWowMoment::GetCurveSplineClosestPoint(float const PlayerSplineDistance) const 
{
	float const ProportionalDistanceOnCameraSpline = (PlayerSplineDistance / PlayerSplineLength) * CameraSplineLength;
	FVector const CameraSplineLocation = CameraSpline->GetLocationAtDistanceAlongSpline(ProportionalDistanceOnCameraSpline, ESplineCoordinateSpace::World);
	
	return CameraSplineLocation;
}

FRotator ACameraSplineWowMoment::GetCurrentRotation(float const PlayerSplineDistance) const
{
	float const ProportionalDistanceOnCameraSpline = (PlayerSplineDistance / PlayerSplineLength) * TargetSplineLength;
	
	FVector const TargetSplineLocation = TargetSpline->GetLocationAtDistanceAlongSpline(ProportionalDistanceOnCameraSpline, ESplineCoordinateSpace::World);
	FVector const LookAtDirection = TargetSplineLocation - m_TargetLocation;
	
	return FRotationMatrix::MakeFromX(LookAtDirection).Rotator();
}
