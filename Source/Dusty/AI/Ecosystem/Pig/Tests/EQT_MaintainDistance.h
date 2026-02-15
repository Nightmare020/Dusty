#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "EQT_MaintainDistance.generated.h"


UCLASS()
class DUSTY_API UEQT_MaintainDistance : public UEnvQueryTest
{
	GENERATED_BODY()

public:
	UEQT_MaintainDistance(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;

	UPROPERTY(EditDefaultsOnly, Category=Distance)
	TSubclassOf<UEnvQueryContext> DistanceTo;
	
	UPROPERTY(EditDefaultsOnly, Category = "Design")
	float MaxDistance = 1000.f;

	UPROPERTY(EditDefaultsOnly, Category = "Design")
	float MinWalkableDistance = 500.f;
};
