#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../../UI/HUD/HUDSubsystem.h"
#include "../../UI/HUD/InteractiveButtonInterface.h"
#include "../../Utils/DustyUtils.h"
#include "Interactable.generated.h"

class ADustyCharacter;
class UInteractiveButtonInterface;
class UBoxComponent;

UCLASS()
class DUSTY_API AInteractable : public AActor
{
	GENERATED_BODY()
public:
	AInteractable();
	virtual void Activate(bool _bActivation = true);
	virtual void Deactivate() {}
	

	/* Begin overlap function to save game when player overlaps interactable object area */
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
	virtual void Interact() {}
	virtual void ChangeMode() {};

	virtual void OnFinishedAutoMovement() {};


	UPROPERTY(EditAnywhere, Transient)
		UStaticMeshComponent* Mesh = nullptr;

	UPROPERTY(Transient)
		UMaterialInterface* Material = nullptr;

	UPROPERTY(Transient)
		UMaterialInstanceDynamic* DynamicMaterial = nullptr;

	UPROPERTY(Transient)
		ADustyCharacter* DustyCharacter = nullptr;

	UPROPERTY(EditAnywhere)
		float SpawnDespawnAnimationSpeed = 10.f;

	/** Activate function that can be implemented in blueprints. Does not need a declaration in the cpp file */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Toggle Actor", meta = (DisplayName = "Dusty_Activate"))
	void BP_Activate();

protected:
	// Variable to check if the player is in the interaction range
	bool HasPlayerOverlapped = false;

private:
	TArray<UMaterialInstanceDynamic*> m_DynamicMaterials;

	bool m_SpawnAnimation = false;
	bool m_DespawnAnimation = false;

	FVector2D m_OriginalInteractiveButtonInterfaceScale;

};
