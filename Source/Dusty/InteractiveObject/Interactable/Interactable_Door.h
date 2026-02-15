#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "Interactable_Door.generated.h"

UCLASS()
class DUSTY_API AInteractable_Door : public AInteractable
{
	GENERATED_BODY()
	AInteractable_Door();
public:

	virtual void Interact() override;
};
