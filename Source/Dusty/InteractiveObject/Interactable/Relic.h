#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "Relic.generated.h"


UCLASS()
class DUSTY_API ARelic : public AInteractable
{
private:
	GENERATED_BODY()
	
public:
	ARelic();
	virtual void Interact() override;

public:
	FString Name;
	bool IsEnabled = false;
	
};
