#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "MazeNPC_CheckCanMove.generated.h"


UENUM(BlueprintType)
enum class CheckerValue : uint8 {
	CheckMove			    UMETA(DisplayName = "CheckMove"),
	CheckStop			UMETA(DisplayName = "CheckStop"),
};


class AMazeNPC;

UCLASS()
class DUSTY_API UMazeNPC_CheckCanMove : public UAnimNotify
{
private:
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:
	UPROPERTY(Transient)
	AMazeNPC* m_MazeNPC = nullptr;

public:
	UPROPERTY(EditAnywhere, Category = "Design")
	CheckerValue CheckValue = CheckerValue::CheckMove; 
	
	
};
