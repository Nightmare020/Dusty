#pragma once

#include "CoreMinimal.h"
#include "CameraFocusToEvent.h"
#include "CameraFocusToDoor.generated.h"


UCLASS()
class DUSTY_API ACameraFocusToDoor : public ACameraFocusToEvent
{
private:
	GENERATED_BODY()

public:
	virtual void Exited() override;
	
};
