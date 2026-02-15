#pragma once

#include "CoreMinimal.h"
#include "BaseActors/MazeActorTrigger.h"
#include "MazeTriggerToggleActor.generated.h"


class AMazeNPC;
class AMazeTriggerWaypoint;
class ALightActor;
class ACameraFocusToEvent;


UCLASS()
class DUSTY_API AMazeTriggerToggleActor : public AMazeActorTrigger
{
private:
	GENERATED_BODY()
	
public:
	AMazeTriggerToggleActor();
	virtual void BeginPlay() override;
	virtual void DoBeginOverlap(AActor* OtherActor) override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "LightActor")
	ALightActor* m_LightActor = nullptr;
};
