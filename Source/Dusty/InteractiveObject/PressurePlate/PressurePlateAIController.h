#pragma once

#include "CoreMinimal.h"
#include "PressurePlateController.h"
#include "PressurePlateAIController.generated.h"


class AMazeTriggerTargetOverrider;
class AMazeNPC;

UCLASS()
class DUSTY_API APressurePlateAIController : public APressurePlateController
{
private:
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "NPC")
	AMazeNPC* m_NPC = nullptr;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "CanMove")
	// bool m_CanMove = false;
	//
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "NeedsToSeePlayer")
	// bool m_NeedsToSeePlayer = false;
	//
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "UpdatesPath")
	// bool m_UpdatesPath = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "TriggerToActivate")
	AMazeTriggerTargetOverrider* m_TriggerToActivate = nullptr;

public:
	virtual void ResolvePuzzleControl() override;
	
};
