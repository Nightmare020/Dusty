#pragma once

#include "CoreMinimal.h"
#include "DustyCameraModifier.h"
#include "CM_SmoothMovement.generated.h"

class ADustyCharacter;

UCLASS()
class DUSTY_API UCM_SmoothMovement : public UDustyCameraModifier
{
private:
	GENERATED_BODY()

public:
	virtual bool ProcessViewRotation(class AActor* ViewTarget, float DeltaTime, FRotator& OutViewRotation, FRotator& OutDeltaRot) override;
	virtual void DisableModifier(bool bImmediate = false) override;

public:
	UPROPERTY(EditAnywhere, Category = "Design")
		float MaxYawTurnRate = 50.f;
	UPROPERTY(EditAnywhere, Category = "Design")
		float MaxPitchTurnRate = 50.f;
	UPROPERTY(EditAnywhere, Category = "Design", meta = (ToolTip = "Delay to start smooth movement"))
		float DelayBaseTime = 0.5;
	UPROPERTY(EditAnywhere, Category = "Design")
		float PitchBaseLocation = -15.f;
	
private:
	void AngleController();
	void SetPitchBaseLocation(float _pitchBaseLocation) { m_PitchCurrentLocation = PitchBaseLocation + _pitchBaseLocation; }

private:
	float m_LastHeight = 0.f;
	float m_PitchTurnRate = 5.f;
	float m_DelayTimer = 0.5f;
	float m_PitchCurrentLocation = -15.f;
	
};