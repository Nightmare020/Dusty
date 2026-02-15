#pragma once

#include "CoreMinimal.h"
#include "ToolActorBase.h"
#include "HexagonPuzzleCreator.generated.h"


UCLASS()
class DUSTY_API AHexagonPuzzleCreator : public AToolActorBase
{
private:
	GENERATED_BODY()

private:
	void ApplyMesh();
	void GeneratePuzzle();
	void CleanNulls();

protected:
	// refs to things
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool")
	TArray<UStaticMesh*> Meshes;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool")
	class AHexagonPuzzleController* PuzzleController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool")
	TSubclassOf<AActor> ActorToSpawn;

	// hexagon setter
	float Apothem  = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool")
	int Hexagons_X = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool")
	int Hexagons_Y = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool")
	float Radius = 0.f;

	// controls settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool")
	FKey ApplyTextureButton;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool")
	FKey ApplyEraseButton;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool")
	FKey ApplyGenerateButton;
	
public:
#if WITH_EDITORONLY_DATA
	virtual void EditorKeyPressed(FKey Key, EInputEvent Event) override;
#endif
	
};