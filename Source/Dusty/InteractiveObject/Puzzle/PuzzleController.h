#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Dusty/SaveGame/SaveGameInterface.h"
#include "PuzzleController.generated.h"

class APuzzlePiece;

UCLASS(Abstract)
class DUSTY_API APuzzleController : public AActor, public ISaveGameInterface
{
	GENERATED_BODY()
	
public:	
	APuzzleController();
	// Update puzzle depending on player actions towards it
	virtual void UpdatePuzzle(APuzzlePiece* TriggerActor) {};


protected:
	// Array of puzzle pieces that compose the possible puzzle solution
	TArray<APuzzlePiece*> PuzzlePieces;

	virtual void GetPuzzlePieces() {};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle")
		int PuzzleID = 0;
};
