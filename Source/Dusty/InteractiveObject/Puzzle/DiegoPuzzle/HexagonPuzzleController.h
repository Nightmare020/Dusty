#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../PuzzleController.h"
#include "HexagonPuzzleController.generated.h"

class AHexagonPuzzlePiece;

UCLASS()
class DUSTY_API AHexagonPuzzleController : public APuzzleController
{
private:
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = "Design", meta = (ToolTip = "Max piece index + 1"), meta = (AllowPrivateAccess = "true"), DisplayName = "AmountOfLevels")
	int m_AmountOfLevels = 0;

	UPROPERTY(EditAnywhere, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "CheckPointIndex")
	int m_CheckPointIndex = 7;
	
	UPROPERTY(EditAnywhere, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "HeightOfPuzzle")
	float m_HeightOfPuzzle = 0.f;
	
	UPROPERTY(EditAnywhere, Category = "Design", meta = (ToolTip = "Make it funky, make it random"), meta = (AllowPrivateAccess = "true"), DisplayName = "RandomVariationInHeightOfPuzzle")
	float m_RandomVariationInHeightOfPuzzle = 0.f;
	
	UPROPERTY(EditAnywhere, Category = "Design", meta = (ToolTip = "Time it takes to move one piece"), meta = (AllowPrivateAccess = "true"), DisplayName = "BasePillarSpeed")
	float m_BasePillarSpeed = 50.f;
	
	UPROPERTY(EditAnywhere, Category = "Design", meta = (ToolTip = "Random extra time it will take (from 0 to value)"), meta = (AllowPrivateAccess = "true"), DisplayName = "RandomVariationPillarSpeed")
	float m_RandomVariationPillarSpeed = 10.f;

	UPROPERTY(EditAnywhere, Category = "Design", meta = (ToolTip = "Random extra time it will take (from 0 to value)"), meta = (AllowPrivateAccess = "true"), DisplayName = "DescentMultiplier")
	float m_DescentMultiplier= 3.f;
	
	UPROPERTY(EditAnywhere, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "Pìeces")
	TArray<AHexagonPuzzlePiece*> m_Pieces;

	UPROPERTY(EditAnywhere, DisplayName = "Movement Sound")
	USoundBase* m_sound = nullptr;
	int m_Index = 0; // puzzle current level
	
	// controls if puzzle is being resolved correctly (1) or is in reset mode (0)
	bool m_Status = false;

public:
	AHexagonPuzzleController();

	virtual void LoadActor();
	virtual void OnActorSaved_Implementation();
	
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Design")
	virtual void UpdatePuzzle(APuzzlePiece* TriggerActor) override;

	int GetCurrentIndex() const;
	bool GetCurrentStatus() const; 
	
	float GetRandomVariationInHeightOfPuzzle() const { return m_RandomVariationInHeightOfPuzzle; }
	TArray<AHexagonPuzzlePiece*>& GetPieces() { return m_Pieces; }

private:
	bool m_ResetControl = false;
};
