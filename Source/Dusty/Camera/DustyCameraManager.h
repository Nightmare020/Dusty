#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "DustyCameraManager.generated.h"

class ACameraOverrider;
class ADustyCameraActor;
class UCameraComponent;
class ADustyCharacter;
class UCameraModifier;
class ACameraSwitch;

inline float TOLERANCE = 0.01;

UCLASS()
class DUSTY_API ADustyCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	virtual void UpdateCamera(float DeltaTime) override;

	void SetModifier(TSubclassOf<UCameraModifier> ModifierClass, bool _IsActive);
	void SetModifier(bool _IsActive);
	
	void EnableModifiers();
	void DisableModifiers();

	void EnableShakeModifier();
	void DisableShakeModifier();
	bool IsShakeModifierEnabled() { return m_IsEnabledShakeModifier;};

	void SetCurrentOverriderOnWaitValue(bool _newValue);
	bool IsCurrentOverriderOnWait() const;


public:
	UPROPERTY(Transient)
		ADustyCharacter* ControlledCharacter = nullptr;

	UPROPERTY(Transient)
		UCameraComponent* CameraComponent = nullptr;

	UPROPERTY(Transient)
		ADustyCameraActor* CameraActor = nullptr;

	UPROPERTY(Transient)
	ACameraOverrider* CameraOverrider = nullptr;

	bool IsActorCameraActive = false;
	

private:
	UPROPERTY(Transient)
	TSubclassOf<UCameraModifier> m_CurrentCameraModifier = nullptr;
	
	UPROPERTY(Transient)
	class UBaseState* m_LastState = nullptr; // for camera init control

	UPROPERTY(EditAnywhere, Category="Design", meta = (AllowPrivateAccess = "true"), DisplayName = "SpeedCurve")
	UCurveFloat* m_SpeedCurve = nullptr;

	float m_MaxCurveValue = 0;

	//Initial Blur Values
	float Aperture;
	float MaxAperture;
	float DiaphragmBlades;
	float SensorWitdh;
	float FocalDistance;
	float DepthBlurRadius;
	
	bool m_IsEnabledShakeModifier;
#pragma region GlobalVariables
	
public:
	UPROPERTY(EditAnywhere, Category = "Design", meta = (ToolTip = "Value from 0 to 1 representing input vector for X axis so that player controller doesnt react with a tiny input"))
	float InputOverrider = 0.2;
	
	// base values of spring arm
	FVector m_BaseSpringArmSocketOffset;
	FVector m_BaseSpringArmTargetOffset;
	float m_BaseSpringArmLength;
	
	bool m_SpringArmIsCorrectlyPlaced = false;
	bool m_SocketOffsetIsCorrectlyPlaced = false;

	// Max distances for curve speed adapting
	float m_SpringArmLengthMaxDistance = 0.f;
	float m_SpringArmSocketOffsetMaxDistance = 0.f;
	
#pragma endregion
	
#pragma region BasicLocomotionCamera
public:
	void InitBasicLocomotionCamera();
	void ExitBasicLocomotionCamera();
	void UpdateBasicLocomotionCamera(float DeltaTime);

#pragma endregion
	
#pragma region ClimbMovementRegion
public:
	void InitClimbMovementCamera();
	void ExitClimbMovementCamera();
	void UpdateClimbMovementCamera(float DeltaTime);

#pragma endregion
	
#pragma region AimStateRegion
public:
	void InitAimCamera();
	void ExitAimCamera();
	void UpdateAimCamera(float DeltaTime);

#pragma endregion
	
#pragma region PoleJumpingRegion
public:
	void InitPoleJumpingCamera();
	void ExitPoleJumpingCamera();
	void UpdatePoleJumpingCamera(float DeltaTime);

#pragma endregion

#pragma region CameraUtils
public:
	UPROPERTY(EditAnywhere, Category = "Design", DisplayName = "SpringArmMoveSpeed", meta = (AllowPrivateAccess = "true"))
	float BaseSpringArmMoveSpeed = 350.f;
	float m_SpringArmMoveSpeed = 350.f;

	UPROPERTY(EditAnywhere, Category = "Design", DisplayName = "SocketOffsetMoveSpeed", meta = (AllowPrivateAccess = "true"))
	float BaseSocketOffsetMoveSpeed = 200.f;
	float m_SocketOffsetMoveSpeed = 200.f;
	
	UPROPERTY(EditAnywhere, Category = "Design",  DisplayName = "MaxSpringArmLength", meta = (ToolTip = "Deprecated"), meta = (AllowPrivateAccess = "true"))
	float m_MaxSpringArmLength = 425.f;
	
	//Meant for camera book
	void SetCameraActorBlur(bool value);

	void GetTransitioParameters();
	void SpringArmResetter(float DeltaTime);
	void SocketOffsetResetter(float DeltaTime);
	void FovController();
	FRotator GetDeltaRotator() const;
	bool IsOverrided() const;

	// todo: deprecated
	void SpringArmController(float DeltaTime);

	static bool CameraLerp(float const& DeltaTime, float& CurrentValue, float const& TargetValue, float const& Speed);
	static bool CameraLerp(float const& DeltaTime, FVector& CurrentLocation, FVector const& TargetLocation, float const& Speed);
	static bool CameraLerp(float const& DeltaTime, FRotator& CurrentRotation, FRotator const& TargetRotation, float const& Speed);
	
#pragma endregion

#pragma region CameraStart
public:
	void StartCameraInit(const FTransform& _cameraTransform, float _transitionTime);
	void StartCameraExit();

private:
	FVector m_StartCameraCurrentLocation = FVector::ZeroVector;
	FRotator m_StartCameraCurrentRotation = FRotator::ZeroRotator;

	float m_StartCameraTransitionTime = 0.f;

	bool m_IsOnStartCamera = false;
	bool m_IsOnStartCameraWait = true;

	FVector m_StartCameraTargetLocation = FVector::ZeroVector;;
	FRotator m_StartCameraTargetRotation = FRotator::ZeroRotator;

	float m_StartCameraMoveSpeed = 0.f;
	float m_StartCameraRotateSpeed = 0.f;

	bool m_StartCameraReachedLocation = false;
	bool m_StartCameraReachedRotation = false;
	
#pragma endregion

};
