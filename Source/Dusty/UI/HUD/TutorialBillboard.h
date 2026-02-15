#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../../UI/HUD/InteractiveButtonInterface.h"
#include "../../Utils/DustyUtils.h"
#include "TutorialBillboard.generated.h"

//class UInteractiveButtonInterface;
class UBoxComponent;
class UTextRenderComponent;
UCLASS()
class DUSTY_API ATutorialBillboard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATutorialBillboard();

public:
	virtual void BeginPlay() override;
	virtual void Activate(bool _bActivation = true);
	virtual void Deactivate();

	virtual void Tick(float DeltaTime) override;

	/* Begin overlap function to save game when player overlaps interactable object area */
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
	virtual void Interact() {}
	virtual void ChangeMode() {};

	virtual void OnFinishedAutoMovement() {};

	void SpawnAnimation();
	void DespawnAnimation();

	UPROPERTY(EditAnywhere, Transient)
	UStaticMeshComponent* Mesh = nullptr;

	UPROPERTY(Transient)
	UMaterialInterface* Material = nullptr;

	UPROPERTY(Transient)
	UMaterialInstanceDynamic* DynamicMaterial = nullptr;

	UPROPERTY(Transient)
	ADustyCharacter* DustyCharacter = nullptr;

	UPROPERTY(EditAnywhere)
	UBoxComponent* Trigger;

	UPROPERTY(EditAnywhere)
	TArray<UInteractiveButtonInterface*> Billboards;

	UPROPERTY(EditAnywhere)
	TArray<UTextRenderComponent*> Texts;


	UPROPERTY(EditAnywhere)
	float SpawnDespawnAnimationSpeed = 10.f;


	/** Activate function that can be implemented in blueprints. Does not need a declaration in the cpp file */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Toggle Actor", meta = (DisplayName = "Dusty_Activate"))
	void BP_Activate();

protected:
	// Variable to check if the player is in the interaction range
	bool HasPlayerOverlapped = false;

private:
	int m_BillboardsNum = 5; 
	int m_TextsNum = 5; 


	TArray<UMaterialInstanceDynamic*> m_DynamicMaterialsBillboards;
	TArray<UMaterialInstanceDynamic*> m_DynamicMaterialsTexts;

	bool m_SpawnAnimationBillboard = false;
	bool m_DespawnAnimationBillboard = false;

	bool m_SpawnAnimationTexts = false;
	bool m_DespawnAnimationTexts = false;

	UPROPERTY(Transient)
	TArray<FVector2D> m_OriginalBillboardScale;

	UPROPERTY(Transient)
	TArray<FVector2D> m_OriginalTextsScale;

	UPROPERTY(Transient)
	TArray<bool> m_VisibilityBillboard;

	UPROPERTY(Transient)
	TArray<bool> m_VisibilityTexts;
};
