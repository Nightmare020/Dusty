#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Dusty/Utils/DustyConst.h"
#include "Dusty/SaveGame/SaveGameInterface.h"
#include "PressurePlateController.generated.h"

class AToggleActor;
class AInteractable;
class AEventCamera;
class UPageWidget;

UCLASS()
class DUSTY_API APressurePlateController : public AActor, public ISaveGameInterface
{
	GENERATED_BODY()
	
protected:
	virtual void ResolvePuzzleControl();
	
	void UnresolvePuzzle();

	int m_PiecesToResolve = 0;
	bool m_Completed = false;
	bool m_canComplete = false;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "EventCamera")
	AEventCamera* m_EventCamera = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "DissolveActorsInitialValue")
	bool m_DissolveActorsInitialValue = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "DissolveActorsDissolveTime")
	float m_DissolveActorsDissolveTime = 2.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design|Academy", meta = (AllowPrivateAccess = "true"), DisplayName = "DissolveActorsDissolveTime")
	TArray<APressurePlateController*> AuxiliarAcademyControllers;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "Delay Time for Activate")
	float m_DelayToActivate = 0.f;

	FTimerHandle m_delayToActivateHandle;

public:
	void APieceIsSolved();
	void IncreasePiece();

	virtual void ResolvePuzzle();
	virtual void CallToResolvePuzzle();

	/*Toggle Actor*/
	UPROPERTY(EditInstanceOnly, category = "Pressure Plate Controller", DisplayName = "ToggleActors")
	TArray<AToggleActor*> InteractablesList;

	UPROPERTY(EditInstanceOnly, category = "Pressure Plate Controller", DisplayName = "Interactables")
	TArray<AInteractable*> RealInteractablesList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "Controller Only Activates Once")
	bool DoOnce = false;
	UPROPERTY(EditInstanceOnly, Category = "Design")
	TSubclassOf<UPageWidget> PageToUnlock = nullptr;
	UPROPERTY(EditInstanceOnly, Category = "Design")
	bool HastoUnlockGlyph = false;
	UPROPERTY(EditInstanceOnly, Category = "Design", meta = (ToolTip = "Needs PageToUnlock to be different from None/Nullptr"))
	EGlyphs GlyphToUnlock;

	bool HasGivenGlyph = false;

	virtual void BeginPlay() override;

	virtual void LoadActor();

	virtual void OnActorSaved_Implementation();
	
};
