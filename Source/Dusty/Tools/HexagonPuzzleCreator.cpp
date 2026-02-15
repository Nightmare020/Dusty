#include "HexagonPuzzleCreator.h"

#include "Dusty/InteractiveObject/Puzzle/DiegoPuzzle/HexagonPuzzlePiece.h"
#include "Dusty/InteractiveObject/Puzzle/DiegoPuzzle/HexagonPuzzleController.h"

#if WITH_EDITORONLY_DATA
void AHexagonPuzzleCreator::EditorKeyPressed(FKey Key, EInputEvent Event)
{
	Super::EditorKeyPressed(Key, Event);
	if (Key.GetFName() == ApplyTextureButton.GetFName())
	{
		ApplyMesh();
	}

	if (Key.GetFName() == ApplyEraseButton.GetFName())
	{
		CleanNulls();
	}

	if (Key.GetFName() == ApplyGenerateButton.GetFName())
	{
		GeneratePuzzle();
	}
}
#endif

void AHexagonPuzzleCreator::ApplyMesh()
{
#if WITH_EDITORONLY_DATA
	if (PuzzleController)
	{
		CleanNulls();
		for(AHexagonPuzzlePiece* Piece: PuzzleController->GetPieces())
		{
			if (Meshes.IsValidIndex(Piece->PillarType))
			{
				Piece->Pillar->SetStaticMesh(Meshes[Piece->PillarType]);
			}
		}
	}
#endif
}

void AHexagonPuzzleCreator::GeneratePuzzle()
{
#if WITH_EDITORONLY_DATA
	Apothem = Radius * (sqrt(3) / 2);
	FTransform Transform = GetActorTransform();
	FVector Location = GetActorLocation();

	if (PuzzleController)
	{
		// this is maths and geometry to set a grid of hexagons
		for (int i = 0; i < Hexagons_Y; ++i)
		{
			Location = GetActorLocation();

			// for Y axis remember that hexagons start on 1 or 1.5
			if (i % 2 != 0)
			{
				Location.X += Apothem;
			}
			Location.Y += Radius * 1.5 * i;
				
			for (int j = 0; j < Hexagons_X; ++j)
			{
				Transform.SetLocation(Location);
				AActor* NewActor = GEditor->AddActor(GEditor->GetEditorWorldContext().World()->GetCurrentLevel(), ActorToSpawn, Transform);
				// add to controller the pieces
				PuzzleController->GetPieces().Push(Cast<AHexagonPuzzlePiece>(NewActor));
				Location.X += Apothem * 2;
			}
		}
	}
#endif
}

void AHexagonPuzzleCreator::CleanNulls()
{
#if WITH_EDITORONLY_DATA
	// erase all nulls from array in controller
	if (PuzzleController)
	{
		PuzzleController->GetPieces().RemoveAll([](AActor* Element) { return Element == nullptr; });
	}
#endif
}