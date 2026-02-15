#pragma once

#include "CoreMinimal.h"
#include "DustyCameraModifier.h"
#include "CM_ClimbAutoAdjustment.generated.h"


UCLASS()
class DUSTY_API UCM_ClimbAutoAdjustment : public UDustyCameraModifier
{
private:
	GENERATED_BODY()

public:
	virtual bool ProcessViewRotation(class AActor* ViewTarget, float DeltaTime, FRotator& OutViewRotation, FRotator& OutDeltaRot) override;
	virtual void DisableModifier(bool bImmediate = false) override;
	virtual void EnableModifier() override;

public:
	UPROPERTY(EditAnywhere, Category = "Design")
	float MaxTurnRate = 30.f;
	UPROPERTY(EditAnywhere, Category = "Design", meta = (ToolTip = "Delay to start smooth movement"))
	float DelayBaseTime = 0.5;
	UPROPERTY(EditAnywhere, Category = "Design")
	float PitchBaseLocation = -30.f;

private:
	float m_TurnRate = 20.f;
	float m_DelayTimer = 0.5f;
};
