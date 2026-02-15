#pragma once

#include "CoreMinimal.h"
#include "BaseActors/MazeActor.h"
#include "Dusty/AI/Maze/MazeNPC.h"
#include "MazePlayerActivator.generated.h"

class AMazeNPC;
class AMazeTriggerWaypoint;
class AMazeTriggerTargetOverrider;



UCLASS()
class DUSTY_API AMazePlayerActivator : public AMazeActor
{
private:
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "TargetPoint")
	USceneComponent* m_TargetPoint = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "NewState")
	AIType m_NewState = AIType::Guide;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "NeedsToSeePlayer")
	bool m_NeedsToSeePlayer = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "CanMove")
	bool m_CanMove = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "IsAlreadyOn")
	bool m_IsAlreadyOn = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"))
	TArray<AMazeTriggerWaypoint*> PathPoints;
	
	
public:
	AMazePlayerActivator();
	
	virtual void BeginPlay() override;
	void Activate(); // activate method to use in whatever activation system you pick
	
};
