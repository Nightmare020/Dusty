#pragma once

#include "CoreMinimal.h"
#include "BaseActors/MazeActorTrigger.h"
#include "MazeTriggerTargetOverrider.generated.h"

class AMazeNPC;
class UBoxComponent;
class AMazeTriggerWaypoint;
class AMazeAIController;
class ADustyCharacter;

UENUM(BlueprintType)
enum class TriggerType : uint8 {
	Forwards			    UMETA(DisplayName = "Forwards"),
	Backwards			UMETA(DisplayName = "Backwards"),
};


UCLASS()
class DUSTY_API AMazeTriggerTargetOverrider : public AMazeActorTrigger
{
private:
	GENERATED_BODY()

	void OverrideTarget() const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "OtherTrigger")
	AMazeTriggerTargetOverrider* m_OtherTrigger = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "OverridedWaypoint")
	AMazeTriggerWaypoint* m_ControlledWaypoint = nullptr;
	
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "UpdatesPath")
	// bool m_UpdatesPath = true;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "OneUseOnly")
	// bool m_OneUseOnly = false;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "IfStoppedNeedsToSeePlayer")
	// bool m_IfControlledNeedsToSeePlayer = true;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "TriggerType")
	// TriggerType m_TriggerType = TriggerType::Forwards;

	
public:	
	AMazeTriggerTargetOverrider();
	
	virtual void BeginPlay() override;
	virtual void DoBeginOverlap(AActor* OtherActor) override;
	
	//void SetCollision(ECollisionEnabled::Type EnterCollision);

private:
	UPROPERTY(Transient)
	AMazeAIController* m_EntityController = nullptr;

	int m_PathUpdate = 0;

	//void ManagePathPoints();
	void ManageTriggers();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(Transient)
	ADustyCharacter* m_DustyCharacter = nullptr;
	
};