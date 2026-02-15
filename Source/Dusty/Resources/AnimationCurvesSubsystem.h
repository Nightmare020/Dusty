#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "AnimationCurvesSubsystem.generated.h"

class UAnimationCurves;

UCLASS(Abstract, Blueprintable)
class DUSTY_API UAnimationCurvesSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Transient, BlueprintReadWrite, Category = "Animation")
	UAnimationCurves* AnimationCurves;	
};
