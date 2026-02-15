#pragma once

#include "CoreMinimal.h"
#include "CameraFocusToEvent.h"

#include "CameraFocusToInteractingAI.generated.h"


class AMazeTriggerBlockMovement;

UCLASS()
class DUSTY_API ACameraFocusToInteractingAI : public ACameraFocusToEvent
{
private:
	GENERATED_BODY()

public:
	virtual void Reached() override;
	virtual void Exited() override;
	
	void SetTriggerOwner(AMazeTriggerBlockMovement* _owner);

private:
	UPROPERTY(Transient)
	AMazeTriggerBlockMovement* m_Owner = nullptr;
};
