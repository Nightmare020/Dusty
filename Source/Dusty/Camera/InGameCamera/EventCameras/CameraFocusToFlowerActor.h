#pragma once

#include "CoreMinimal.h"
#include "CameraFocusToEvent.h"
#include "CameraFocusToFlowerActor.generated.h"


class AInteractiveFlower;

UCLASS()
class DUSTY_API ACameraFocusToFlowerActor : public ACameraFocusToEvent
{
private:
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "FlowerActor")
	AInteractiveFlower* m_FlowerActor = nullptr;


public:
	virtual void Reached() override;
	virtual void Exited() override;
	void SetActivateValue(bool _associatedToggleActorActivateValue);

public:
	bool m_AssociatedToggleActorActivateValue = false;

	
};
