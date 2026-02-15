#pragma once

#include "CoreMinimal.h"
#include "CameraOverrider.h"
#include "CameraSpline.generated.h"


class UArrowComponent;
class USplineComponent;
class ADustyCharacter;

UCLASS(Abstract)
class DUSTY_API ACameraSpline : public ACameraOverrider
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Design", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Trigger = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"))
	USplineComponent* CameraSpline = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "ExitArrow")
	UArrowComponent* m_ExitArrow = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "EntryArrow")
	UArrowComponent* m_EntryArrow = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Design", meta = (AllowPrivateAccess = "true"))
	float BaseBlendMoveSpeed = 300.f;
	
	UPROPERTY(EditAnywhere, Category = "Design", meta = (AllowPrivateAccess = "true"))
	float BaseBlendRotationSpeed = 100.f;

	UPROPERTY(EditAnywhere, Category = "Design", meta = (AllowPrivateAccess = "true"))
	float BlendDelayTime = 1.f;

	UPROPERTY(EditAnywhere, Category = "Design", meta = (AllowPrivateAccess = "true"))
	float BlendTargetFOV = 90.f;

	UPROPERTY(EditAnywhere, Category="Design", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* SpeedCurve = nullptr;

	UPROPERTY(EditAnywhere, Category="Design", meta = (AllowPrivateAccess = "true"))
	float ExitPitchValue = -20.f;
	
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "OneUseOnly")
	bool m_OneUseOnly = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "MaintainInputDirection")
	bool m_RememberInputDirection = false;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "OneUseOnlyTime")
	// float m_OneUseOnlyTime = 2.f;
	
	
protected:
#pragma region DefaultVariables
	
	bool m_Mode = false; // true (to spline), false (to player)
	bool m_MaintainInputDirection = false;
	
	float MaxCurveValue = 0.f;

	FRotator m_StoredControlRotation;
	
	FVector m_StartingLocation;
	FRotator m_StartingRotation;

	FVector m_LastCameraLocation;
	FRotator m_LastCameraRotation;

	// factor added for camera splines that require special speeds (see Helix Spline)
	float PlayerSpeedFactor = 1.f;
	
#pragma endregion

#pragma region UpdateVariables
	
	FVector m_CurrentLocation;
	FRotator m_CurrentRotation;
	float m_CurrentFOV;
	
	FVector m_TargetLocation;
	FRotator m_TargetRotation;
	float m_TargetFOV;

	float m_BlendMoveSpeed;
	float m_BlendRotationSpeed;
	float m_BlendFOVSpeed;

	float m_BlendTimer = 0.f;
	//float m_OneUseOnlyTimer = 0.f;
	
	bool m_InPlace = true;
	bool m_NewState = false;
	bool m_ReachedLocation = false;
	bool m_ReachedRotation = false;
	bool DustyActuallyInTrigger = false;

	bool m_TimerReady = false;
	
#pragma endregion
	
public:	
	ACameraSpline();

	virtual void BeginPlay() override;
	virtual void Update(float DeltaTime) override;
	virtual void Tick(float DeltaTime) override;
	virtual FRotator GetInputVector() override;

	UFUNCTION()
		void TriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void TriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	virtual void SetBlendParameters();
	virtual void SetBlendTargets();
	void GetDistances(float &RotationDistance, float &MovementDistance, float &FOVDistance) const;
	float GetNeededCameraMoveSpeed() const;
};