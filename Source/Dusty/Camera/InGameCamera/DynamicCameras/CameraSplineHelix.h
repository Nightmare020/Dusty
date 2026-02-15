#pragma once

#include "CoreMinimal.h"
#include "CameraSpline.h"
#include "CameraSplineHelix.generated.h"


UCLASS()
class DUSTY_API ACameraSplineHelix : public ACameraSpline
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"))
	float PlayerSplineRadius;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"))
	float CameraSplineRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"))
	float ExitBlendMaxMoveSpeed = 450.f;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"))
	USplineComponent* PlayerSpline = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"))
	float CorrectionStrength = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "InputDirection")
	float m_InputDirection = -1;
	
public:
	ACameraSplineHelix();

	virtual void SetBlendTargets() override;
	virtual void SetBlendParameters() override;
	virtual FRotator GetInputVector() override;

private:
	float m_SplinePosition = 0.f;
	
};