#pragma once

#include "CoreMinimal.h"
#include "EventCamera.h"
#include "CameraFade.generated.h"


class ADustyCameraManager;
class APressurePlateController;
class ADustyCharacter;
class UArrowComponent;

UCLASS()
class DUSTY_API ACameraFade : public AEventCamera
{
public:
	ACameraFade();

private:
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "TargetArrow")
	UArrowComponent* m_TargetArrow = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "FadeTime")
	float m_FadeTime = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "FocusTime")
	float m_FocusTime = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "ExitPitchValue")
	float m_ExitPitchValue = -20.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "ExitYawValue")
	float m_ExitYawValue = 0.f;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	virtual void Init(bool _IsFollowUpCamera = false) override;

	virtual void Reached() override;
	virtual void Exited() override;

private:
	void FadeControl();
	void FocusControl();
	
private:
	int m_Current = 0;
	float m_FocusTimer = 0.f;

	FTimerHandle m_TimeHandle;
	
};
