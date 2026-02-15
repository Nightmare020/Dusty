#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "EnvironmentQuery/Tests/EnvQueryTest_Distance.h"
#include "EQT_DistanceWithPathfinding.generated.h"


UCLASS()
class DUSTY_API UEQT_DistanceWithPathfinding : public UEnvQueryTest
{
private:
	GENERATED_BODY()

public:
	explicit UEQT_DistanceWithPathfinding();
	virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;

public:
	/** context */
	UPROPERTY(EditDefaultsOnly, Category=Distance)
	TSubclassOf<UEnvQueryContext> DistanceTo;
};
