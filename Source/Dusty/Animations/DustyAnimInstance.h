#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "../Character/DustyCharacter.h"
#include "../Character/DustyMovementComponent.h"
#include "../Utils/DustyConst.h"
#include "DustyAnimInstance.generated.h"


class ADustyCharacter;
class UDustyMovementComponent;


UCLASS()
class DUSTY_API UDustyAnimInstance : public UAnimInstance
{
private:
	GENERATED_BODY()

private:

#pragma region Debugs
private:
	UPROPERTY(BlueprintReadWrite, DisplayName = "ShowDebugs", meta = (AllowPrivateAccess = "true"))
	bool m_showDebugs;

	UPROPERTY(BlueprintReadWrite, DisplayName = "DebugArrowStartPos", meta = (AllowPrivateAccess = "true"))
	FVector m_debugArrowStartPos;
#pragma endregion

#pragma region LocomotionValues
private:
	UPROPERTY(BlueprintReadWrite, DisplayName = "DesiredPlayerAngle", meta = (AllowPrivateAccess = "true"))
	float m_desiredPlayerAngle;

	UPROPERTY(BlueprintReadWrite, DisplayName = "DesiredPlayerRotation", meta = (AllowPrivateAccess = "true"))
	FVector m_desiredPlayerRotation;

	UPROPERTY(BlueprintReadWrite, DisplayName = "WalkStartAngle", meta = (AllowPrivateAccess = "true"))
	float m_walkStartAngle;

	UPROPERTY(BlueprintReadWrite, DisplayName = "GroundVelocity", meta = (AllowPrivateAccess = "true"))
	FVector m_groundVelocity;

	UPROPERTY(BlueprintReadWrite, DisplayName = "GroundVelocityMagnitude", meta = (AllowPrivateAccess = "true"))
	float m_groundVelocityMagnitude;

	UPROPERTY(BlueprintReadWrite, DisplayName = "StaticJump", meta = (AllowPrivateAccess = "true"))
	bool m_staticJump;

#pragma endregion

#pragma region AnimGraphValues
private:
	UPROPERTY(BlueprintReadWrite, DisplayName = "BlendStrafeAlpha", meta = (AllowPrivateAccess = "true"))
	float m_BlendStrafeAlpha;

	UPROPERTY(BlueprintReadWrite, DisplayName = "BlendAimAlpha", meta = (AllowPrivateAccess = "true"))
	float m_BlendAimAlpha;

	UPROPERTY(BlueprintReadWrite, DisplayName = "BlendStartStickAlpha", meta = (AllowPrivateAccess = "true"))
	float m_BlendStartStickAlpha;

	UPROPERTY(BlueprintReadWrite, DisplayName = "BlendStartStickMagnetismAlpha", meta = (AllowPrivateAccess = "true"))
	float m_BlendStartStickMagnetismAlpha;

	UPROPERTY(BlueprintReadWrite, DisplayName = "AoVerticalAim", meta = (AllowPrivateAccess = "true"))
	float m_AoVerticalAim;

	UPROPERTY(BlueprintReadWrite, DisplayName = "AoHorizontalAim", meta = (AllowPrivateAccess = "true"))
	float m_AoHortizontalAim;

	UPROPERTY()
	FVector2D m_AoAimInertia;

	UPROPERTY(BlueprintReadWrite, DisplayName = "TiltedHorizontal", meta = (AllowPrivateAccess = "true"))
	float m_TitledHortizontal;

	UPROPERTY()
	FVector2D m_TiltedInertia;

	UPROPERTY(BlueprintReadWrite, DisplayName = "CurrentPlayerState", meta = (AllowPrivateAccess = "true"))
	PlayerStates m_CurrentPlayerState;

	UPROPERTY(BlueprintReadWrite, DisplayName = "RotationInput", meta = (AllowPrivateAccess = "true"))
	FVector2D m_RotationInput;

	UPROPERTY(BlueprintReadWrite, DisplayName = "ShouldUseIkRig", meta = (AllowPrivateAccess = "true"))
	bool m_ShouldUseIkRig;
#pragma endregion


#pragma region BonesValues
private:
	UPROPERTY(BlueprintReadWrite, DisplayName = "BonePelvisOffset", meta = (AllowPrivateAccess = "true"))
	FVector m_bonePelvisOffset;

	UPROPERTY(BlueprintReadWrite, DisplayName = "BoneFootROffset", meta = (AllowPrivateAccess = "true"))
	FVector m_boneFootROffset;

	UPROPERTY(BlueprintReadWrite, DisplayName = "BoneFootLOffset", meta = (AllowPrivateAccess = "true"))
	FVector m_boneFootLOffset;

	UPROPERTY(BlueprintReadWrite, DisplayName = "BoneFootRRotation", meta = (AllowPrivateAccess = "true"))
	FRotator m_boneFootRRotation;

	UPROPERTY(BlueprintReadWrite, DisplayName = "BoneFootLRotation", meta = (AllowPrivateAccess = "true"))
	FRotator m_boneFootLRotation;

	UPROPERTY(BlueprintReadWrite, DisplayName = "BoneHeadRotation", meta = (AllowPrivateAccess = "true"))
	FRotator m_boneHeadRotation;

	UPROPERTY(BlueprintReadWrite, DisplayName = "PositionToLookAt", meta = (AllowPrivateAccess = "true"))
	FVector m_positionToLookAt;

#pragma endregion

	/** Reference to the player Dusty Character */
	UPROPERTY(transient)
	ADustyCharacter* m_DustyCharacter;

	/** Reference to the player Dusty Movement Component */
	UPROPERTY(transient)
	UDustyMovementComponent* m_DustyMovementComponent;

	/** Dusty Ground Speed when he's on the walk or run state */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Reference, meta = (AllowPrivateAccess = "true"))
	float m_GroundSpeed;

	/** Dusty Air Speed when he's in the falling, jumping or climbing state */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Reference, meta = (AllowPrivateAccess = "true"))
	float m_AirSpeed;

	/** Check if Dusty should move when he's on the idle state */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Reference, meta = (AllowPrivateAccess = "true"))
	bool m_ShouldMove;

	/** Check if Dusty is on falling state */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Reference, meta = (AllowPrivateAccess = "true"))
	bool m_IsFalling;

	/** Check if Dusty is going forward after a pole elevator */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Reference, meta = (AllowPrivateAccess = "true"))
	bool m_ForwardPoleJump;	
	/** Check if Dusty is going forward after a pole elevator */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Reference, meta = (AllowPrivateAccess = "true"))
	bool m_InputAfterPoleJump;	


public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	/** Function to manage ground speed variable */
	UFUNCTION(BlueprintCallable, Category = "Animation Flow")
	void GetGroundSpeed();

	/** Function to manage air speed variable */
	UFUNCTION(BlueprintCallable, Category = "Animation Flow")
	void GetAirSpeed();

	/** Function to manage the should move boolean variable */
	UFUNCTION(BlueprintCallable, Category = "Animation Flow")
	void GetShouldMove();

	/** Function to manage the is falling boolean variable */
	UFUNCTION(BlueprintCallable, Category = "Animation Flow")
	void GetIsFalling();

	bool GetIsStaticJump();
	void SetIsStaticJump(bool jump = true);

	bool GetForwardPoleElevator();
	void SetForwardPoleElevator(bool forward = true);

	bool GetInputAfterPoleJump();
	void SetInputAfterPoleJump(bool Input = true);

	/** Function to play an specific anim montage */
	UFUNCTION(BlueprintCallable, Category = "Animation Player")
	void PlayMontage(UAnimMontage* MontageToPlay);

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	void CalculateGroundValues();

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	void CalculateRotationValues();

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	void CalculateAimValues();

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	void CalculateTiltedValues();

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	void DeterminePlayerState();

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	void AddAlphaStickCube(float value);

	void SetDustyLookAt(FVector Position);

	void RemoveDustyLookAt();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "lightGrass")
	bool m_lightGrass;


	UFUNCTION()
	void AnimNotify_ExtendPoleElevator();
	UFUNCTION()
	void AnimNotify_PoleKeyInserted();
	UFUNCTION()
	void AnimNotify_PoleKeyRemoved();
	UFUNCTION()
	void AnimNotify_EndInitialJumpInPoleJump();
	UFUNCTION()
	void AnimNotify_StartExtendingBottomPole();
	UFUNCTION()
	void AnimNotify_ResetInputAfterPoleJump();
};
