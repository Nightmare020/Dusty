#pragma once

#include "CoreMinimal.h"
#include "HexagonPuzzleController.h"
#include "HexagonPuzzleAIController.generated.h"


class AMazeNPC;
class AMazeAIController;

UCLASS()
class DUSTY_API AHexagonPuzzleAIController : public AHexagonPuzzleController
{
private:
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "NPC")
	AMazeNPC* m_NPC = nullptr;


public:
	virtual void BeginPlay() override;
	virtual void UpdatePuzzle(APuzzlePiece* TriggerActor) override;
	void SetNumberOfPillars(int _numberOfPillars);

private:
	TArray<AHexagonPuzzlePiece*> CorrectPillars;
	
	UPROPERTY(Transient)
	AMazeAIController* m_AIController = nullptr;

	int m_NumberOfDifferentPillars = 5;
};
