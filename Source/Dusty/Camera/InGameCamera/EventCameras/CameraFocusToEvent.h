#pragma once

#include "CoreMinimal.h"
#include "Dusty/Camera/InGameCamera/EventCameras/EventCamera.h"
#include "CameraFocusToEvent.generated.h"

class AToggleActor;
class UArrowComponent;
class ADustyCharacter;

UCLASS()
class DUSTY_API ACameraFocusToEvent : public AEventCamera
{
	GENERATED_BODY()
	
public:	
	ACameraFocusToEvent();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "TransitionTime")
	float m_TransitionTime = 3.f;

	UPROPERTY(EditAnywhere, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "FocusTime")
	float m_FocusTime = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "ExitPitchValue")
	float m_ExitPitchValue = -20.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "ModifyCurrentState")
	bool m_ModifyCurrentState= true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "BlockPlayersInput")
	bool m_BlockPlayersInput= false;
	
	UPROPERTY(EditDefaultsOnly, Category = "Design", meta = (AllowPrivateAccess = "true"))
	UArrowComponent* TargetArrow = nullptr;
	
private:
	float m_Timer;
	bool m_ReachedLocation = false;
	bool m_ReachedRotation = false;

	FVector m_TargetLocation;
	FRotator m_TargetRotation;

	FVector m_CurrentLocation;
	FRotator m_CurrentRotation;

	float m_MoveSpeed;
	float m_RotateSpeed;

	ECameraState m_Mode;

	bool m_BugFix = false;

private:
	void SetBlendSpeeds();

public:	
	virtual void Update(float DeltaTime) override;

	// bool only used when toggle actor associated to camera
	virtual void Init(bool _IsFollowUpCamera = false) override;
	virtual void Exit() override;

	virtual void Reached() override; // method in case there is any action required when reached target
	virtual void Exited() override; // method in case there is a deactivate action like disabling collisions

	float GetTransitionTime() const;
	
};
