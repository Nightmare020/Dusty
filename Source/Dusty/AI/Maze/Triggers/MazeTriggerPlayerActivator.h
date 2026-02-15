#pragma once

#include "CoreMinimal.h"
#include "MazePlayerActivator.h"
#include "BaseActors/MazeActorTrigger.h"
#include "MazeTriggerPlayerActivator.generated.h"


UCLASS()
class DUSTY_API AMazeTriggerPlayerActivator : public AMazeActorTrigger
{
private:
	GENERATED_BODY()

	void SetControlledActorsIsEnabled();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "TargetPoint")
	USceneComponent* m_TargetPoint = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "NewState")
	AIType m_NewState = AIType::Guide;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "NeedsToSeePlayer")
	bool m_NeedsToSeePlayer = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "CanMove")
	bool m_CanMove = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "IsLighted")
	bool m_IsLighted = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "IsAlreadyOn")
	bool m_IsAlreadyOn = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"))
	TArray<AMazeTriggerWaypoint*> PathPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "MazeActorsToActivate")
	TArray<AMazeActor*> m_MazeActorsToActivate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "MazeActorsToDeactivate")
	TArray<AMazeActor*> m_MazeActorsToDeactivate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "TriggerActivatorToDisable")
	AMazeTriggerPlayerActivator* m_TriggerActivatorToDisable = nullptr;


public:
	AMazeTriggerPlayerActivator();

	virtual void BeginPlay() override;

	virtual void DoBeginOverlap(AActor* OtherActor) override;

public:
	bool NeedsToActivatedOnBeginPlay = false;
	
};
