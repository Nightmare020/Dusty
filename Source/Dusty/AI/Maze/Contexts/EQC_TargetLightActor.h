#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "EQC_TargetLightActor.generated.h"

class AMazeNPC;

UCLASS()
class DUSTY_API UEQC_TargetLightActor : public UEnvQueryContext
{
private:
	GENERATED_BODY()

public:
	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;
	
	
};
