#pragma once

#include "CoreMinimal.h"
#include "CameraSpline.h"
#include "CameraSplineCurveToSpline.generated.h"


UCLASS()
class DUSTY_API ACameraSplineCurveToSpline : public ACameraSpline
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"))
	USplineComponent* PlayerSpline = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"))
	USplineComponent* TargetSpline = nullptr;


public:
	ACameraSplineCurveToSpline();
	virtual void SetBlendTargets() override;
	FVector GetCurveSplineClosestPoint() const;

	
};
