#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraModifier.h"
#include "DustyCameraModifier.generated.h"

class ADustyCameraManager;
class ADustyCharacter;

UCLASS(Abstract)
class DUSTY_API UDustyCameraModifier : public UCameraModifier
{
private:
	GENERATED_BODY()

public:
	void Init(ADustyCameraManager* _DustyCameraManager, ADustyCharacter* _ControlledCharacter);
	virtual void EnableModifier() override;
	
protected:
	UPROPERTY(Transient)
	ADustyCameraManager* DustyCameraManager = nullptr;

	UPROPERTY(Transient)
	ADustyCharacter* ControlledCharacter = nullptr;

public:
	UPROPERTY(EditAnywhere, Category = "Design|Constraints")
		float ViewPitchMin = -89.9;

	UPROPERTY(EditAnywhere, Category = "Design|Constraints")
		float ViewPitchMax = 30.0;

	UPROPERTY(EditAnywhere, Category = "Design|Constraints")
		float ViewYawMin = 0.f;

	UPROPERTY(EditAnywhere, Category = "Design|Constraints")
		float ViewYawMax = 359.9;
	
	UPROPERTY(EditAnywhere, Category = "Design|Constraints")
		float BaseSpringArmLength = 400.f;

	UPROPERTY(EditAnywhere, Category = "Design|Constraints")
		FVector BaseSpringArmSocketOffset = FVector(25.f, 60.f, 75.0);

	UPROPERTY(EditAnywhere, Category = "Design|Constraints")
		FVector BaseSpringArmTargetOffset = FVector(0.f, 0.f, 0.f);
	
};
