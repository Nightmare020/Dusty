#include "CaterpillarSpline.h"

#include "Components/SplineComponent.h"

ACaterpillarSpline::ACaterpillarSpline()
{
	PrimaryActorTick.bCanEverTick = false;
	
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("Root"));
	
	m_Spline = CreateDefaultSubobject<USplineComponent>("Spline");
	m_Spline->SetupAttachment(RootComponent);
}

void ACaterpillarSpline::BeginPlay()
{
	Super::BeginPlay();
	
}

FVector ACaterpillarSpline::GetNextLocation(const FVector& CurrentLocation, const float DistanceToMove) const
{
	const float SplineLocation = m_Spline->FindInputKeyClosestToWorldLocation(CurrentLocation);
	float NewSplineLocation = SplineLocation + DistanceToMove / m_Spline->GetSplineLength();

	if (NewSplineLocation > m_Spline->GetNumberOfSplinePoints() - 1) 
	{
		NewSplineLocation -= m_Spline->GetNumberOfSplinePoints() - 1; // Wrap around
	}
	
	const FVector NewLocation = m_Spline->GetLocationAtSplineInputKey(NewSplineLocation, ESplineCoordinateSpace::World);
	
	return NewLocation;
}

FVector ACaterpillarSpline::GetClosestLocation(const FVector& CurrentLocation) const
{
	return m_Spline->FindLocationClosestToWorldLocation(CurrentLocation,  ESplineCoordinateSpace::World);
}

FVector ACaterpillarSpline::GetClosestLocation(const float SplineLocation) const
{
	// clamp and get spline length
	const float ClampedPercentage = FMath::Clamp(SplineLocation, 0.0f, 1.0f);
	const float SplineLength = m_Spline->GetSplineLength();

	// get distance along spline and then FVector
	const float DistanceInSpline = ClampedPercentage * SplineLength;
	return m_Spline->GetLocationAtDistanceAlongSpline(DistanceInSpline, ESplineCoordinateSpace::World);
	
}

FRotator ACaterpillarSpline::GetCurrentRotation(const FVector& CurrentLocation) const
{
	const float SplineLocation = m_Spline->FindInputKeyClosestToWorldLocation(CurrentLocation);
	return m_Spline->GetRotationAtSplineInputKey(SplineLocation, ESplineCoordinateSpace::World);
}
