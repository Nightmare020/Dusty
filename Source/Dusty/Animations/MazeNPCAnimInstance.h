#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MazeNPCAnimInstance.generated.h"

class AMazeNPC;

UCLASS()
class DUSTY_API UMazeNPCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	UPROPERTY(Transient)
	AMazeNPC* m_MazeNPC;
public:
	
	virtual void NativeInitializeAnimation() override;

	UFUNCTION()
	void AnimNotify_CheckCanMove();
};
