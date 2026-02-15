// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "Dusty/SaveGame/SaveGameInterface.h"
#include "PressurePlate.generated.h"

class APressurePlateController;
class UNiagaraComponent;

UCLASS()
class DUSTY_API APressurePlate : public ATriggerBox, public ISaveGameInterface
{
	GENERATED_BODY()
public:
	APressurePlate();


	virtual void LoadActor();
	virtual void OnActorSaved_Implementation();


protected:
	UPROPERTY(EditInstanceOnly)
	bool m_IsMachinePressurePlate = false;

	bool m_DustyPressingPressurePlate = false;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

	virtual void Activated();
	virtual void Deactivated();

private:
	FVector m_OriginPos = FVector(0,0,0); //Plate Mesh original pose
	FVector m_ActivatePos = FVector(0, 0, 0); //Plate Mesh original pose -m_pressed dis on Z
	bool m_IsActive = false;
	unsigned int m_SomethingOnTop = 0;
	float m_SpeedTValue = 0; //Lerp Helper

	UPROPERTY(EditAnywhere, category = "Pressure Plate", meta = (AllowPrivateAccess = "true"))
	float m_PressSpeed = 1.5f;

protected:
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* PlateMesh;
	UPROPERTY(EditDefaultsOnly)
		UNiagaraComponent* NiagaraCmp = nullptr;

	UPROPERTY(EditDefaultsOnly, category = "Pressure Plate", meta=(DisplayName = "Plate Pressed Distance", Tooltip = "Distance the plate will go down on Z axis when pressed/activated"))
	float PressedDis = 50.f; //Distance plate mesh goes down when the plate is being pressed
	
	UPROPERTY(EditInstanceOnly, category = "Pressure Plate")
	APressurePlateController* ControllerInstance = nullptr;
	UPROPERTY(EditInstanceOnly, category = "Pressure Plate")
	APressurePlateController* AdditionalControllerInstance = nullptr;

	UPROPERTY(EditAnywhere)
	USoundBase* m_pressSound;
};
