#pragma once

#include "CoreMinimal.h"

#include "InteractiveRoot.h"
#include "Camera/CameraComponent.h"
#include "Dusty/InteractiveObject/Light/TimedLightActor.h"
#include "InteractiveFlower.generated.h"

class ACameraFocusToEvent;
class ACameraFocusToFlowerActor;
class UAnimSequence;
class ACameraActor;
class AInteractable;
class UNiagaraComponent;
class UNiagaraSystem;

UCLASS()
class DUSTY_API AInteractiveFlower : public ATimedLightActor
{
	GENERATED_BODY()

public:
	AInteractiveFlower();

	UPROPERTY(EditAnywhere, Category = "Design | Roots")
	TArray<AInteractiveRoot*> ControlledRoots;

	UPROPERTY(EditAnywhere, Category = "Design | Roots")
	TArray<AInteractable*> InteractablesToActivate;
	
	UPROPERTY(EditAnywhere, Category = "Design | Roots")
	USkeletalMeshComponent* Petals;

	// only used by those with a camera
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design | Camera", meta = (ToolTip = "Only used by those with a camera associated"))
	ACameraFocusToFlowerActor* FocusCameraToFLower = nullptr;

	virtual void Tick(float DeltaTime) override;

	virtual void Activate(bool _Activating) override;
	virtual void Activate(bool _bActivation, AActor* _actor) override;

	UMaterialInstanceDynamic* m_material;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool Close;

	UPROPERTY(Transient)
	UCameraComponent* Camera;

	virtual void BeginPlay() override;

	virtual void LoadActor() override;

private:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

	UPROPERTY(EditAnywhere)
		UBoxComponent* m_Trigger;

	UPROPERTY(EditAnywhere)
		int m_FirstButtonId = 3;
	UPROPERTY(EditAnywhere)
		int m_SecondButtonId = 2;

	// Did this for readability :)
	const float m_ScaleFactor = 5000;

	const float m_Extended = 1;

	const float m_Retracted = 0;

	float m_CurrentRootsValue;

	float m_StartRootsValue;

	float m_FinishRootsValue;

	float m_Timer = 0.f;

	// only used for flowers with camera
	bool m_IsWaitingForCamera = false;

	UPROPERTY(EditAnywhere)
	USoundBase* m_sound;

	UPROPERTY(EditAnywhere)
	USoundBase* m_lightSound;
	UPROPERTY(EditAnywhere)
	UNiagaraComponent* m_niagaraEffect;
	
	UPROPERTY(EditAnywhere, Transient)
	UNiagaraSystem* m_effect = nullptr;

	UPROPERTY(EditAnywhere)
	bool m_ignoreAction = false;
};