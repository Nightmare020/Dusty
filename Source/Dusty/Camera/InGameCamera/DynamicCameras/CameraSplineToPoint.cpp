#include "CameraSplineToPoint.h"

#include "Camera/CameraComponent.h"
#include "Components/SplineComponent.h"
#include "Dusty/Camera/DustyCameraManager.h"
#include "Dusty/Character/DustyCharacter.h"


ACameraSplineToPoint::ACameraSplineToPoint()
{
	TargetPoint = CreateDefaultSubobject<USceneComponent>("POILocation");
	TargetPoint->SetupAttachment(RootComponent);
}


void ACameraSplineToPoint::SetBlendTargets()
{
	if (m_Mode) // if targeting fixed
	{
		m_TargetLocation = CameraSpline->FindLocationClosestToWorldLocation(Player->GetActorLocation(), ESplineCoordinateSpace::World);
		FVector const LookAtDirection = TargetPoint->GetComponentLocation() - m_TargetLocation;
		m_TargetRotation = FRotationMatrix::MakeFromX(LookAtDirection).Rotator();
	}
	else // if targeting player
	{
		m_TargetLocation = Player->CameraManager->CameraComponent->GetComponentLocation();
		FVector const LookAtDirection = Player->CameraManager->CameraComponent->GetForwardVector();
		m_TargetRotation = FRotationMatrix::MakeFromX(LookAtDirection).Rotator();
	}
}
