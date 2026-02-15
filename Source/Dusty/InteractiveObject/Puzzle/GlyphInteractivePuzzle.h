#pragma once

#include "CoreMinimal.h"
#include "../Interactable/Interactable.h"
#include "GlyphInteractivePuzzle.generated.h"

class UTextRenderComponent;
class AInteractiveFlower;

UCLASS()
class DUSTY_API AGlyphInteractivePuzzle : public AInteractable
{
private:
	GENERATED_BODY()
	
	virtual void Interact() override;

public:
	UFUNCTION(BlueprintImplementableEvent)
	void SwapText();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Text;
	
	UPROPERTY(EditAnywhere, Transient, BlueprintReadWrite)
	AInteractiveFlower* InteractiveFlower = nullptr;

	UPROPERTY(EditAnywhere)
	TArray<AActor*> m_Roots;

	UPROPERTY(EditAnywhere)
	bool m_SkyGlyph;
};
