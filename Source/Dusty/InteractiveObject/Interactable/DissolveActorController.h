#pragma once

#include "CoreMinimal.h"
#include "Dusty/InteractiveObject/Light/TimedLightActor.h"
#include "DissolveActorController.generated.h"


class AEventCamera;
class UNiagaraSystem;
class UNiagaraComponent;
class APlanetsKeyController;
class ADissolveActor;

UCLASS()
class DUSTY_API ADissolveActorController : public ATimedLightActor
{
private:
	GENERATED_BODY()

	UPROPERTY(Transient)
	UMaterialInstanceDynamic* m_dynamicMaterial;

	bool m_sphereGoingUp = false;
	bool m_sphereGoingDown = false;

	UPROPERTY(Transient)
	AActor* m_StoredActor = nullptr;
	bool m_StoredActivateValue = false;
	bool m_IfCameraReady = false;
	bool m_IfCameraReadyWithActor = false;

	FVector m_sphereInitialPos;
	
	UPROPERTY(EditAnywhere)
	UNiagaraComponent* m_niagaraComponent;
	UPROPERTY(EditAnywhere, DisplayName = "Effect")
	UNiagaraSystem* m_niagaraSystem;

	UPROPERTY(EditAnywhere)
	USoundBase* m_particleSound;
	
public:
	ADissolveActorController();

	virtual void Activate() override;
	virtual void Activate(bool _bActivation) override;
	virtual void Activate(bool _bActivation, AActor* _actor) override;

	virtual void BeginPlay() override;

	void UpdateMaterialsAndSpherePhysics();

	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Design | Light")
	UStaticMeshComponent* ControlBox = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Design | Light")
	UStaticMeshComponent* ControlSphere = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Design | Light")
	USceneComponent* SpehereDownPos = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "EventCamera")
	AEventCamera* m_EventCamera = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "IsFollowUpEvent")
	bool m_IsFollowUpEvent = false;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design | Light")
	TArray<ADissolveActor*> DissolveEntities;

	UPROPERTY(EditAnywhere)
	APlanetsKeyController* planetkeyController;
	
};
