// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Utils/DustyConst.h"
#include "PoleJumpConstraintActor.generated.h"

class UPhysicsConstraintComponent;
class ADustyCharacter;
class UPoseableMeshComponent;


struct PoleJumpValuesStruct
{
public:
	FVector PolePoint = FVector::ZeroVector;
	FVector PoleImpulse = FVector::ZeroVector;
	FVector PoleDirection = FVector::ZeroVector;
	float TimeToExtend = 0.3f;
	float AngleToRetract = 45.f;
	float MovementFactor = 1000.f;
	float ScaleFactor = 1.f;
	bool NeedsToBeExtendedForMovement = false;
	float ExtendScale = 6.f;
	EPoleMovementType PoleMovementType = EPoleMovementType::Jump;


	PoleJumpValuesStruct(FVector _PolePoint, FVector _PoleImpulse, FVector _PoleDirection, float _TimeToExtend, float _AngleToRetract, float _MovementFactor, float _ScaleFactor = 1.f, bool _NeedsToBeExtendedForMovement = false, float _ExtendScale = 6.f, EPoleMovementType _PoleMovementType = EPoleMovementType::Jump)
		: PolePoint(_PolePoint),
		PoleImpulse(_PoleImpulse),
		PoleDirection(_PoleDirection),
		TimeToExtend(_TimeToExtend),
		AngleToRetract(_AngleToRetract),
		MovementFactor(_MovementFactor),
		ScaleFactor(_ScaleFactor),
		NeedsToBeExtendedForMovement(_NeedsToBeExtendedForMovement),
		ExtendScale(_ExtendScale),
		PoleMovementType(_PoleMovementType)
	{
	}
};

UCLASS()
class DUSTY_API APoleJumpConstraintActor : public AActor
{
	GENERATED_BODY()

private:
	//Variables
	USoundBase* m_poleSound = LoadObject<USoundBase>(nullptr, TEXT("/Game/Audio/Tracks/Testuing/Palo_Contraer.Palo_Contraer"));
	
	UPROPERTY(EditDefaultsOnly)
	UPhysicsConstraintComponent* m_Constraint;

	UPROPERTY(EditDefaultsOnly)
	USceneComponent* m_DustyPosition;

	UPROPERTY(EditDefaultsOnly)
	USceneComponent* m_TopPolePosition;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UPoseableMeshComponent* m_PoleSK;

	float m_DistanceToTop = 0.f;

	UPROPERTY(Transient)
	ADustyCharacter* m_DustyCharacter = nullptr;

	bool m_IsExtending = false;
	bool m_IsExtended = false;

	UPROPERTY(EditDefaultsOnly, Category = Blend)
	FAlphaBlend m_PoleJumpingExtensionBlend;

	float m_CurrentScale = 1.f;
	float m_DesiredScale = 1.f;

	float m_InitialBottomExtension = 0.f;
	float m_DesiredExtension = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Design | PoleJump")
	float m_EndImpulse = 425.f;

	UPROPERTY(EditDefaultsOnly, Category = "Design | PoleJump")
	float m_InitialRotationSpeed = 20.f;
	UPROPERTY(EditDefaultsOnly, Category = "Design | PoleJump")
	float m_MaxRotationSpeed = 50.f;
	UPROPERTY(EditDefaultsOnly, Category = "Design | PoleJump")
	float m_BaseTimeToMaxRotation = 0.4f;
	UPROPERTY(EditDefaultsOnly, Category = "Design | PoleJump")
	float m_MaxJumpDistance = 700.f;
	float m_TimeToMaxRotation = m_BaseTimeToMaxRotation;

	float m_TimerToMaxRotation = 0.f;



	FVector m_Impulse = FVector::ZeroVector;
	FVector m_LastPolePosition = FVector::ZeroVector;
	FVector m_StartingMovementDirection = FVector::ZeroVector;

	UPROPERTY(EditDefaultsOnly, Category = "Design | PoleJump")
	float m_DustyForwardJumpOffset = 40.f;
	UPROPERTY(EditDefaultsOnly, Category = "Design | PoleJump")
	float m_DustyRightJumpOffset = -15.f;
	UPROPERTY(EditDefaultsOnly, Category = "Design | PoleJump")
	float m_DustyForwardElevatorOffset = -5.f;
	UPROPERTY(EditDefaultsOnly, Category = "Design | PoleJump")
	float m_DustyRightElevatorOffset = 0.f;
	UPROPERTY(EditDefaultsOnly, Category = "Design | PoleJump")
	float m_DustyUpElevatorOffset = 41.3f;
	UPROPERTY(EditDefaultsOnly, Category = "Design | PoleJump")
	float m_ForwardAngleThreshold = 45.f;

	FVector m_FinalDirection = FVector::ZeroVector;

	//Functions
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

	void FixDustyPosition();

	PoleJumpValuesStruct* m_PoleJumpValues;
	UFUNCTION()
	void OnPoleHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void EndJump();
	bool m_AttachDustyToPole = false;
	bool m_CanReceiveInput = false;

	UPROPERTY(EditDefaultsOnly, Category = "Design | PoleJump")
	float m_RotationThreshold = 1;
	UPROPERTY(EditDefaultsOnly, Category = "Design | PoleJump")
	float m_ExtendOffset = 100;

public:	
	// Sets default values for this actor's properties
	APoleJumpConstraintActor();

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* Pole;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* Pivot;


	//Functions
	void Jump();
	void Move(FVector _MovementVector);
	void Extend(bool _NewExtension);
	void SetPoleJumpValues(PoleJumpValuesStruct* _PoleJumpValues);
	void ExtendBottom(float _Percentage);
	float GetExtensionTime();
	void ActivatePoleLight();
	FVector GetOffsetFromDustyPosition();

};
