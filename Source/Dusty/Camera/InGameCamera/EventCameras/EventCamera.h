#pragma once

#include "CoreMinimal.h"
#include "Dusty/Camera/InGameCamera/DynamicCameras/CameraOverrider.h"
#include "EventCamera.generated.h"


UCLASS()
class DUSTY_API AEventCamera : public ACameraOverrider
{
private:
	GENERATED_BODY()

public:
	virtual void Init(bool _IsFollowUpCamera = false) {}
	virtual void Exit();

	virtual void Reached() {}
	virtual void Exited() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "OneUseOnly")
	bool m_OneUseOnly = false;
	
	UPROPERTY(EditInstanceOnly, Category = "Design", meta = (AllowPrivateAccess = "true"))
	AEventCamera* NextCamera = nullptr;
	
};
