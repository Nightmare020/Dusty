#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "EQC_NextWaypoint.generated.h"

UCLASS()
class DUSTY_API UEQC_NextWaypoint : public UEnvQueryContext
{
private:
	GENERATED_BODY()

public:
	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;
};
