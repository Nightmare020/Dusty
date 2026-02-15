#include "TadpoleController.h"

#include "TadpoleNPC.h"
#include "TadpoleSpline.h"

ATadpoleController::ATadpoleController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATadpoleController::BeginPlay()
{
	Super::BeginPlay();

	for (ATadpoleNPC* Tadpole : m_Tadpoles)
	{
		if (Tadpole)
		{
			Tadpole->SetController(this);
		}
	}
}

void ATadpoleController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

ATadpoleSpline* ATadpoleController::GetTargetSpline(ATadpoleNPC* _Tadpole, ATadpoleSpline* _Current)
{
	float Distance = FLT_MAX;
	FVector TadpoleLocation = _Tadpole->GetActorLocation();
	
	ATadpoleSpline* TargetSpline = nullptr;

	// get the closest spline using the closest point to it as measurement
	for (ATadpoleSpline* Spline : m_Splines)
	{
		// if not null and not equal to current spline
		if (Spline && _Current != Spline)
		{
			FVector TempVector = Spline->GetClosestLocation(TadpoleLocation, _Tadpole->SwimDirection);
			float TempDistance = FVector::Dist(TadpoleLocation, TempVector);

			if(TempDistance < Distance)
			{
				Distance = TempDistance;
				TargetSpline = Spline;
			}
		}	
	}
	
	return TargetSpline;
}

ATadpoleSpline* ATadpoleController::GetRandomSpline(ATadpoleSpline* _Current)
{
	int32 StartingIndex = FMath::RandRange(0, m_Splines.Num() - 1);
	ATadpoleSpline* Target = m_Splines[StartingIndex];

	// if you get a winner first try nice!
	if (Target && Target != _Current)
	{
		return Target;
	}

	// index control for the loop ahead
	int32 CurrentIndex = StartingIndex + 1;
	
	while (CurrentIndex != StartingIndex)
	{
		if(m_Splines.IsValidIndex(CurrentIndex))
		{
			// if found return it
			Target = m_Splines[CurrentIndex];
			if (Target && Target != _Current)
			{
				return Target;
			}
			CurrentIndex++;
		}
		else // reset index to zero to continue loop from the array start
		{
			CurrentIndex = 0;
		}
	}

	// if none found return current again
	return _Current;
}