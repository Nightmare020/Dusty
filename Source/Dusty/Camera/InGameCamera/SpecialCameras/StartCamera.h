#pragma once

#include "CoreMinimal.h"
#include "Dusty/Camera/InGameCamera/EventCameras/EventCamera.h"
#include "StartCamera.generated.h"

class UArrowComponent;

UCLASS()
class DUSTY_API AStartCamera : public AEventCamera
{
private:
	GENERATED_BODY()

public:
	virtual void Init(bool _IsFollowUpCamera) override;
	virtual void Exit() override;

	virtual void Update(float DeltaTime) override;

private:
	void SetBlendSpeeds();

protected:
	UPROPERTY(EditAnywhere, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "TransitionTime")
	float m_TransitionTime = 3.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "ExitPitchValue")
	float m_ExitPitchValue = -20.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Design", meta = (AllowPrivateAccess = "true"))
	UArrowComponent* m_TargetArrow = nullptr;
	
private:
	bool m_IsOnWait = false;

	FVector m_TargetLocation;
	FRotator m_TargetRotation;

	FVector m_CurrentLocation;
	FRotator m_CurrentRotation;

	float m_MoveSpeed;
	float m_RotateSpeed;

	bool m_ReachedLocation = false;
	bool m_ReachedRotation = false;
	
};
