#include "TadpoleSpline.h"

#include "Components/SplineComponent.h"

ATadpoleSpline::ATadpoleSpline()
{
	PrimaryActorTick.bCanEverTick = false;
	
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("Root"));
	
	m_Spline = CreateDefaultSubobject<USplineComponent>("Spline");
	m_Spline->SetupAttachment(RootComponent);
}

void ATadpoleSpline::BeginPlay()
{
	Super::BeginPlay();
	
}

FVector ATadpoleSpline::GetNextLocation(const FVector& CurrentLocation, const float DistanceToMove) const
{
	const float SplineLocation = m_Spline->FindInputKeyClosestToWorldLocation(CurrentLocation);
	const float SplineLength = m_Spline->GetSplineLength();
	float NewSplineLocation = SplineLocation + DistanceToMove / SplineLength;

	// if spline finished send a zero vector to warn
	if (NewSplineLocation > m_Spline->GetNumberOfSplinePoints() - 1 || NewSplineLocation < 0.f) 
	{
		return FVector::ZeroVector;
	}
	
	const FVector NewLocation = m_Spline->GetLocationAtSplineInputKey(NewSplineLocation, ESplineCoordinateSpace::World);
	
	return NewLocation;
}

FVector ATadpoleSpline::GetClosestLocation(const FVector& CurrentLocation, float& Direction) const
{
	FVector NewTarget = m_Spline->FindLocationClosestToWorldLocation(CurrentLocation,  ESplineCoordinateSpace::World);

	const float SplineLocation = m_Spline->FindInputKeyClosestToWorldLocation(CurrentLocation);
	const float HalfTotalPoints = m_Spline->GetNumberOfSplinePoints() / 2.f;

	// it means we are entering the spline from the forward direction
	if(HalfTotalPoints > SplineLocation)
	{
		Direction = 1;
	}
	else
	{
		Direction = -1;
	}
	
	return NewTarget;
}

FVector ATadpoleSpline::GetClosestLocation(const float SplineLocation) const
{
	// clamp and get spline length
	const float ClampedPercentage = FMath::Clamp(SplineLocation, 0.0f, 1.0f);
	const float SplineLength = m_Spline->GetSplineLength();

	// get distance along spline and then FVector
	const float DistanceInSpline = ClampedPercentage * SplineLength;
	return m_Spline->GetLocationAtDistanceAlongSpline(DistanceInSpline, ESplineCoordinateSpace::World);
	
}

FRotator ATadpoleSpline::GetCurrentRotation(const FVector& CurrentLocation) const
{
	const float SplineLocation = m_Spline->FindInputKeyClosestToWorldLocation(CurrentLocation);
	return m_Spline->GetRotationAtSplineInputKey(SplineLocation, ESplineCoordinateSpace::World);
}
