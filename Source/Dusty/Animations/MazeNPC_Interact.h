#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "MazeNPC_Interact.generated.h"

class AMazeNPC;

UCLASS()
class DUSTY_API UMazeNPC_Interact : public UAnimNotify
{
private:
	GENERATED_BODY()
	
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:
	UPROPERTY(Transient)
	AMazeNPC* m_MazeNPC = nullptr;
	
	
};
