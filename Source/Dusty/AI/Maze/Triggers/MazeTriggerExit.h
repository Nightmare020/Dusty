#pragma once

#include "CoreMinimal.h"
#include "BaseActors/MazeActorTrigger.h"
#include "MazeTriggerExit.generated.h"

class AMazeNPC;
class AMazeTriggerPlayerActivator;
class AMazeActor;

UCLASS()
class DUSTY_API AMazeTriggerExit : public AMazeActorTrigger
{
private:
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Design", meta = (AllowPrivateAccess = "true"))
	USceneComponent* m_TargetPoint = nullptr;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "MazeActorsToDeactivate")
	TArray<AMazeActor*> m_MazeActorsToDeactivate;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "TriggerActivatorToDisable")
	AMazeTriggerPlayerActivator* m_TriggerActivatorToDisable = nullptr;

public:
	AMazeTriggerExit();
	
	virtual void BeginPlay() override;
	
	virtual void DoBeginOverlap(AActor* OtherActor) override;

};
