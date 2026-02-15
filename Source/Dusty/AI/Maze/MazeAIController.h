#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MazeAIController.generated.h"


class AMazeNPC;
class UCrowdFollowingComponent;

UCLASS()
class DUSTY_API AMazeAIController : public AAIController
{
private:
	GENERATED_BODY()

public:
	AMazeAIController(FObjectInitializer const& ObjectInitializer);
	virtual void OnPossess(APawn* InPawn) override;

	virtual void BeginPlay() override;

public:
	UPROPERTY(Transient)
	AMazeNPC* ControllerOwner = nullptr;
	
};
