#pragma once

#include "CoreMinimal.h"
#include "KeyController.h"
#include "PlanetsKeyController.generated.h"

class AEventCamera;
class AForceLookCamera;
class APlanetsOrbit;
class ARayPlanet;
class APlanetaryCamera;
class AStaticMeshActor;
class AExponentialHeightFog;

UCLASS()
class DUSTY_API APlanetsKeyController : public AKeyController
{
	GENERATED_BODY()
	APlanetsKeyController();
protected:
	virtual void BeginPlay() override;

	int mCurrentPlanet = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "EventCamera")
	AEventCamera* m_EventCamera = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "BasePlanetaryCamera")
	APlanetaryCamera* m_BasePlanetaryCamera = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "InitCameraOnPoleInserted")
	bool m_InitCameraOnPoleInserted = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "SpeedCurve")
	UCurveFloat* m_SpeedCurve = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Design")
	TArray<AStaticMeshActor*> m_OrbitMeshes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "PlatformBaseSpeed")
	float m_PlatformBaseSpeed = 200.f;
	
	UPROPERTY(EditAnywhere, Category = "Design")
		bool m_LittleFlotation = false;
		bool m_ActiveLittleFlotation = false;

	UPROPERTY(EditDefaultsOnly, Category = "Art")
		float m_Frequency = 0.5;
	float m_FloatingTimer = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Art")
		float m_Amplitude = 20;



	UPROPERTY(EditAnywhere)
		bool m_ChangesFog = false;

	UPROPERTY(Transient)
		AExponentialHeightFog* MapExponentialHeightFog;

	float m_PreviousFogDensity = 0;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void Update(float _DeltaTime, FVector2D _Input) override;
	
	virtual void ChangeMode() override;

	bool NeedsToRotatePlanets = false;

	void CameraSetted();
	void CameraInFade(bool _Start);
	void SetCamera(AEventCamera* _newCamera);


	UPROPERTY(EditAnywhere)
	TArray<ARayPlanet*> RayPlanets;
	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* platform;
	
	bool elevating = false;
	bool control = false;

	// platform location control
	UPROPERTY(EditAnywhere, meta = (MakeEditWidget = true))
	FVector FinalPlatformPosition = FVector::ZeroVector;

	float CurrentSpeed = 1000.f;
	float MaxDistance = 0.f;

	bool controlPlanets = false;
	bool ReachedTarget = false;
	bool Activated = false;

	UPROPERTY(EditAnywhere)
	UAudioComponent* m_platformSound;

public:
	void ChangePlanetsTargets();

	virtual void OnPoleRemoved() override;
	virtual void OnPoleInserted() override;

	virtual void LoadActor() override;
	virtual void OnActorSaved_Implementation() override;
};
