#pragma once

#include "CoreMinimal.h"
#include "../ToggleActor.h"
#include "../Interactable/Interactable.h"
#include "GlyphTogglePuzzle.generated.h"

UCLASS()
class DUSTY_API AGlyphTogglePuzzle : public AToggleActor
{
	GENERATED_BODY()
	
public:
	virtual void Activate() override;
	virtual void Deactivate() override;
	
public:
	UPROPERTY(EditAnywhere, Transient)
	AInteractable* Interactable = nullptr;
};
