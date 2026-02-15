#pragma once

#include "CoreMinimal.h"
#include "CameraSpline.h"
#include "CameraSplineWowMoment.generated.h"


class UArrowComponent;

UCLASS()
class DUSTY_API ACameraSplineWowMoment : public ACameraSpline
{
	GENERATED_BODY()
	
public:
	ACameraSplineWowMoment();

	virtual void BeginPlay() override;
	
	virtual FRotator GetInputVector() override;
	virtual void SetBlendTargets() override;
	
	FVector GetCurveSplineClosestPoint(float const PlayerSplineDistance) const;
	FRotator GetCurrentRotation(float const PlayerSplineDistance) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"))
	USplineComponent* PlayerSpline = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"))
	USplineComponent* TargetSpline = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"))
	UArrowComponent* InputDirection = nullptr;

private:
	float PlayerSplineLength;
	float CameraSplineLength;
	float TargetSplineLength;
};
