// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ToggleActor.h"
#include "RayPlanet.generated.h"


class APlanetsKeyController;
class UNiagaraComponent;

UCLASS()
class DUSTY_API ARayPlanet : public AToggleActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	USceneComponent* PseudoRoot;
	UPROPERTY(EditAnywhere)
	USceneComponent* Root;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* PlanetMesh;

	UPROPERTY(EditAnywhere)
	bool m_InitialEmitting = false;

	bool m_Emitting = false;
	bool m_Rotating = false;

	float m_TimerToRotatePuzzle = 0;
	float m_TimeToRotatePuzzle = 1;

	FVector m_InitialDirection;
	FVector m_DesiredDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess = true))
		UNiagaraComponent* m_RayNiagara = nullptr;

	FVector m_NiagaraDirection;


	UPROPERTY(EditAnywhere, meta = (MakeEditWidget = true))
	TArray<FVector> m_RayDirection;

	float m_CurrentRayDistance = 0.f;

	UPROPERTY(EditAnywhere)
	float m_FirstRayDistance = 1000.f;
	UPROPERTY(EditAnywhere)
	float m_SecondRayDistance = 1000.f;
	UPROPERTY(EditAnywhere)
	float m_AngleThreshold = 15.f;

	FEmissionDirections* m_EmissionDirection = nullptr;

	bool rotatingOrbit;
	bool rotatingPlanet;
	
	float m_InitialRotation;
	float m_DesiredRotation;
	UPROPERTY(EditAnywhere)
	float m_rotationAmount;

	UPROPERTY(EditAnywhere, Category = "Design")
	float m_TimeToRotate = 0.5;
	float m_Timer = 0;

	FVector FloatPosition;

	UPROPERTY(EditAnywhere, Category = "Art")
		float m_Frequency = 0.5;
	float m_FloatingTimer = 0;

	UPROPERTY(EditAnywhere, Category = "Art")
		float m_Amplitude = 50;

public:
	UPROPERTY(Transient)
		APlanetsKeyController* PlanetsKeyController;
	ARayPlanet();
	int m_currentTarget;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void Activate(bool Activation) override;

	void DeactivateOtherActors();
	
	void SetOutline(bool Outline);

	void TranslatePlanet(bool _direction);
	void RotatePlanet(bool _direction);

	void RotatePuzzleCompleted();
	
	virtual void LoadActor();
	virtual void OnActorSaved_Implementation();
};
