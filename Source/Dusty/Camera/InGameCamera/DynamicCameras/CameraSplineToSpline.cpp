#include "CameraSplineToSpline.h"

#include "Camera/CameraComponent.h"
#include "Components/SplineComponent.h"
#include "Dusty/Camera/DustyCameraManager.h"
#include "Dusty/Character/DustyCharacter.h"


ACameraSplineToSpline::ACameraSplineToSpline()
{
	TargetSpline = CreateDefaultSubobject<USplineComponent>("TargetSpline");
	TargetSpline->SetupAttachment(RootComponent);
}

void ACameraSplineToSpline::SetBlendTargets()
{
	if (m_Mode) // if targeting fixed
	{
		m_TargetLocation = CameraSpline->FindLocationClosestToWorldLocation(Player->GetActorLocation(), ESplineCoordinateSpace::World);
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
