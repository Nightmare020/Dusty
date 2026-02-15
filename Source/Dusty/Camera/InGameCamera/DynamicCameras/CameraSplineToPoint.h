#pragma once

#include "CoreMinimal.h"
#include "CameraSpline.h"
#include "CameraSplineToPoint.generated.h"


UCLASS()
class DUSTY_API ACameraSplineToPoint : public ACameraSpline
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Design", meta = (AllowPrivateAccess = "true"))
	USceneComponent* TargetPoint = nullptr;

public:
	ACameraSplineToPoint();
	virtual void SetBlendTargets() override;
	
};
