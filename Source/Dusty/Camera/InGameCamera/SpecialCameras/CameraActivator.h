#pragma once

#include "CoreMinimal.h"
#include "Dusty/Camera/InGameCamera/DynamicCameras/CameraOverrider.h"
#include "CameraActivator.generated.h"

class ACameraOverrider;

UCLASS()
class DUSTY_API ACameraActivator : public ACameraOverrider
{
private:
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "Trigger")
	UStaticMeshComponent* m_Trigger = nullptr;

	UPROPERTY(EditInstanceOnly, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "CameraToActivate")
	ACameraOverrider* m_CameraToActivate = nullptr;

	UPROPERTY(EditInstanceOnly, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "ActivateValue")
	bool m_ActivateValue = true;

	UPROPERTY(EditInstanceOnly, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "OneUseOnly")
	bool m_OneUseOnly = true;
	
public:
	ACameraActivator();

	virtual void BeginPlay() override;
	
	UFUNCTION()
	void TriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
};
