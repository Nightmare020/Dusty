#include "PigNPCAreaController.h"

#include "PigNPC.h"
#include "PigNPCArea.h"

APigNPCAreaController::APigNPCAreaController()
{
	PrimaryActorTick.bCanEverTick = false;
}

void APigNPCAreaController::BeginPlay()
{
	Super::BeginPlay();
	
}

APigNPCArea* APigNPCAreaController::GetClosestArea(const APigNPC* Actor)
{
	float MinDistance = MAX_FLT;

	const FVector ActorLocation = Actor->GetActorLocation();
	APigNPCArea* SelectedArea = nullptr;
	
	for (APigNPCArea* Area : m_TargetAreas)
	{
		if (Area)
		{
			// only check those areas that coincide with the actor substate
			if (Area->GetType() == Actor->CurrentSubState)
			{
				// if area is free
				if(Area->IsFree())
				{
					// if area is closer than selected area then swap selected area
					const float TempDistance = FVector::Dist(ActorLocation, Area->GetActorLocation());
					if (TempDistance < MinDistance)
					{
						MinDistance = TempDistance;
						SelectedArea = Area;
					}
				}
			}
		}
	}

	// set selected area as occupied
	if (SelectedArea)
	{
		SelectedArea->SetIsFree(false);
	}
	
	return SelectedArea;
}