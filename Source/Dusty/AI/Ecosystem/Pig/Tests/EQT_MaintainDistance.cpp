#include "EQT_MaintainDistance.h"

#include "EnvironmentQuery/Contexts/EnvQueryContext_Querier.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_VectorBase.h"


UEQT_MaintainDistance::UEQT_MaintainDistance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
    DistanceTo = UEnvQueryContext_Querier::StaticClass();
	Cost = EEnvTestCost::Low;
	ValidItemType = UEnvQueryItemType_VectorBase::StaticClass();
}

void UEQT_MaintainDistance::RunTest(FEnvQueryInstance& QueryInstance) const
{
	UObject* QueryOwner = QueryInstance.Owner.Get();
	AActor* OwnerActor = Cast<AActor>(QueryInstance.Owner.Get());
	
	if (QueryOwner == nullptr || OwnerActor == nullptr)
	{
		return;
	}
	
	FloatValueMin.BindData(QueryOwner, QueryInstance.QueryID);
	float MinThresholdValue = FloatValueMin.GetValue();

	FloatValueMax.BindData(QueryOwner, QueryInstance.QueryID);
	float MaxThresholdValue = FloatValueMax.GetValue();

	// don't support context Item here, it doesn't make any sense
	TArray<FVector> ContextLocations;
	if (!QueryInstance.PrepareContext(DistanceTo, ContextLocations))
	{
		return;
	}

	for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
	{
		const FVector ItemLocation = GetItemLocation(QueryInstance, It.GetIndex());

		float MinDistance = FLT_MAX;
		for (const FVector& ContextLocation : ContextLocations)
		{
			float Distance = FVector::Dist(ItemLocation, ContextLocation);
			MinDistance = FMath::Min(Distance, MinDistance);
		}

		if (MinDistance < MaxDistance)
		{
			It.SetScore(TestPurpose, FilterType, 0.f, 0.f, 1.f);
		}
		else
		{
			It.SetScore(TestPurpose, FilterType, FVector::Dist(OwnerActor->GetActorLocation(), ItemLocation), 0.f, MinWalkableDistance);
		}
	}
}
