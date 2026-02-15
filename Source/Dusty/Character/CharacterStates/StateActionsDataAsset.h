#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BaseState.h"

#include "StateActionsDataAsset.generated.h"


USTRUCT(BlueprintType)
struct FAvailableStates
{
public:
    GENERATED_BODY()
	
    UPROPERTY(EditAnywhere)
        TArray<TSubclassOf<UBaseState>> AvailableStates;
};


UCLASS()
class DUSTY_API UStateActionsDataAsset : public UDataAsset
{
private:
	GENERATED_BODY()
	
public:

    UPROPERTY(EditAnywhere) 
     TMap<TSubclassOf<UBaseState>, FAvailableStates> StatePriorityMap;
};
