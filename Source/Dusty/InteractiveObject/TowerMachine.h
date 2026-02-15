// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ToggleActor.h"
#include "TowerMachine.generated.h"

class UNiagaraComponent;
class ADissolveActorController;
class ACameraFocusToEvent;
class UAudioComponent;

enum TowerState
{
	Dormant,
	StonesToMachine,
	LightingUp,
	SendingEnergy,
	OpeningBridge,
	Complete
};

UCLASS()
class DUSTY_API ATowerMachine : public AToggleActor
{
	GENERATED_BODY()

private:
	float m_timer = 0;
	TowerState state = Dormant;

	UPROPERTY(EditAnywhere, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "StonesToMachineTimer")
		float m_stonesToMachineTimer = 1;
	UPROPERTY(EditAnywhere, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "LightingUpTimer")
		float m_lightingUpTimer = 1;
	UPROPERTY(EditAnywhere, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "SendingEnergyTimer")
		float m_sendingEnergyTimer = 1;
	UPROPERTY(EditAnywhere, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "OpeningBridgeTimer")
		float m_openingBridgeTimer = 1;

	//UPROPERTY(EditAnywhere, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "OpeningBridgeTimer")
		float m_initialLifetime = 7;

	UPROPERTY(Transient)
	UMaterialInstanceDynamic* m_dynamicMaterial;

protected:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MachineSphere = nullptr;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MachineMonolith = nullptr;
	UPROPERTY(EditAnywhere)
	UNiagaraComponent* MachineStonesA = nullptr;
	UPROPERTY(EditAnywhere)
	UNiagaraComponent* MachineStonesB = nullptr;
	UPROPERTY(EditAnywhere)
	UNiagaraComponent* MachineActiveLights = nullptr;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Design")
	ADissolveActorController* ControllerToLight = nullptr;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Design")
	ACameraFocusToEvent* Camera = nullptr;

	UPROPERTY(EditAnywhere)
	UAudioComponent* m_audioComponent;

public:
	ATowerMachine();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void Activate() override;
	virtual void Deactivate() override;
};
