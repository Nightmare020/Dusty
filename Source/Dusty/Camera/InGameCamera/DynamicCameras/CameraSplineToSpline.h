#pragma once

#include "CoreMinimal.h"
#include "CameraSpline.h"
#include "CameraSplineToSpline.generated.h"


UCLASS()
class DUSTY_API ACameraSplineToSpline : public ACameraSpline
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Design", meta = (AllowPrivateAccess = "true"))
	USplineComponent* TargetSpline = nullptr;

public:
	ACameraSplineToSpline();
	virtual void SetBlendTargets() override;
};
