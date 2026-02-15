#pragma once

#include "CoreMinimal.h"
#include "EventCamera.h"
#include "ForceLookCamera.generated.h"


class UArrowComponent;

UCLASS()
class DUSTY_API AForceLookCamera : public AEventCamera
{
private:
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "LookTarget")
	UArrowComponent* m_TargetArrow = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "RotateSpeed")
	float m_RotateSpeed = 60.f;

public:
	AForceLookCamera();
	
	virtual void Update(float DeltaTime) override;

	virtual void Init(bool _IsFollowUpCamera = false) override;
	virtual void Exit() override;
	
};
