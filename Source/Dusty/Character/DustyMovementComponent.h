// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "DustyMovementComponent.generated.h"

DECLARE_DELEGATE(FOnEnterClimbState)
DECLARE_DELEGATE(FOnExitClimbState)

class ADustyCharacter;
class UAnimMontage;
class UDustyAnimInstance;


UENUM(BlueprintType)
enum ECustomMovementMode
{
	CMOVE_None	   UMETA(Hidden),
	CMOVE_Climb    UMETA(DisplayName = "Climb"),
	CMOVE_PoleJump UMETA(DisplayName = "PoleJump"),
	CMOVE_MAX	   UMETA(Hidden)
};

UCLASS()
class DUSTY_API UDustyMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	UDustyMovementComponent();

	// Stop movement by setting velocity and acceleration automatically to 0
	void StopMovement();

	void SetVelocity(FVector _NewVelocity);

	/** Get an instance to the animation controller of the character */
	UPROPERTY()
		UDustyAnimInstance* DustyPlayerAnimInstance;

private:
	UPROPERTY(Transient)
		ADustyCharacter* m_DustyCharacterOwner;

	virtual void InitializeComponent() override;
	virtual void CalcVelocity(float DeltaTime, float Friction, bool bFluid, float BrakingDeceleration) override;
	virtual float GetMaxSpeed() const override;
	virtual float GetMaxBrakingDeceleration() const override;
	virtual float GetMaxAcceleration() const override;

	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;
	UFUNCTION(BlueprintPure)
		bool IsCustomMovementMode(ECustomMovementMode Mode) const;
	UFUNCTION(BlueprintPure)
		bool IsMovementMode(EMovementMode InMovementMode) const;



#pragma region Climb

public:

	bool CanStartClimbing();
	void StartClimbing();

#pragma endregion

};