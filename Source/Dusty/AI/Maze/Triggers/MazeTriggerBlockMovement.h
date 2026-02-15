#pragma once

#include "CoreMinimal.h"
#include "BaseActors/MazeActorTrigger.h"
#include "MazeTriggerBlockMovement.generated.h"


class ACameraFocusToInteractingAI;

UCLASS()
class DUSTY_API AMazeTriggerBlockMovement : public AMazeActorTrigger
{
private:
	GENERATED_BODY()
	
public:
	virtual void DoBeginOverlap(AActor* OtherActor) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "CameraFocusToInteractingAI")
	ACameraFocusToInteractingAI* m_EventCamera = nullptr;
	
};
