#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "MazeNPC_CanMove.generated.h"

class AMazeNPC;

UCLASS()
class DUSTY_API UMazeNPC_CanMove : public UAnimNotify
{
private:
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:
	UPROPERTY(Transient)
	AMazeNPC* m_MazeNPC = nullptr;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", DisplayName = "CanMove")
	bool m_CanMove = true;
	
};
