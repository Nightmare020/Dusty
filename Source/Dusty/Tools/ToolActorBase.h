#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ToolActorBase.generated.h"

UCLASS()
class DUSTY_API AToolActorBase : public AActor
{
	GENERATED_BODY()
	
protected:	
	AToolActorBase();

	virtual bool ShouldTickIfViewportsOnly() const override;

};