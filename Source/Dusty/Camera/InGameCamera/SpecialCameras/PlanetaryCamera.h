#pragma once

#include "CoreMinimal.h"
#include "Dusty/Camera/InGameCamera/DynamicCameras/CameraOverrider.h"
#include "Dusty/Camera/InGameCamera/EventCameras/EventCamera.h"
#include "PlanetaryCamera.generated.h"


class ADustyCameraManager;
class UArrowComponent;
class ADustyCharacter;
class APlanetsKeyController;

UCLASS()
class DUSTY_API APlanetaryCamera : public AEventCamera
{
public:
	APlanetaryCamera();
	
	virtual void Init(bool _IsFollowUpCamera = false) override;
	virtual void Exit() override;

	virtual void Reached() override;
	virtual void Exited() override;

private:
	GENERATED_BODY()
	
	void FadeControl();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "TargetArrow")
	UArrowComponent* m_TargetArrow = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "FadeTime")
	float m_FadeTime = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "ExitPitchValue")
	float m_ExitPitchValue = -20.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "Field of View")
	float m_FOV = 145.f;

private:
	FTimerHandle m_TimeHandle;

	UPROPERTY(EditInstanceOnly, Category = "Design")
	APlanetsKeyController* m_PlanetsKeyController;
	
	int m_Current = 0;

	bool m_CurrentDesiredInput = false;

	float m_CurrentFov = 0.f; // store current fov to transition from there

	bool m_IsFollowUpCamera = false;
	
};
