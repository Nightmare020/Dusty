#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "PigTargetArea.generated.h"

UCLASS()
class DUSTY_API UPigTargetArea : public UEnvQueryContext
{
private:
	GENERATED_BODY()

public:
	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;
	
};
