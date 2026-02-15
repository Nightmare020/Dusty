#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../ToggleActor.h"
#include "PuzzleController.h"
#include "PuzzlePiece.generated.h"


UCLASS(Abstract)
class DUSTY_API APuzzlePiece : public AToggleActor
{
	GENERATED_BODY()
	
public:	
	APuzzlePiece();

	UPROPERTY(Transient)
	APuzzleController* PuzzleController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle")
		int PuzzleID = 0;
};
