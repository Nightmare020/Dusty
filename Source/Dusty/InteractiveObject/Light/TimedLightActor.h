#pragma once

#include "CoreMinimal.h"
#include "LightActor.h"
#include "TimedLightActor.generated.h"


UCLASS()
class DUSTY_API ATimedLightActor : public ALightActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Design")
	float Duration = 2.f;
	
};
