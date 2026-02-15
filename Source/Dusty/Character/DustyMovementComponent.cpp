// Fill out your copyright notice in the Description page of Project Settings.


#include "DustyMovementComponent.h"
#include "DustyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "CharacterStates/ActionsFilter.h"
#include "TimerManager.h"
#include "../Animations/DustyAnimInstance.h"
#include "../Utils/DustyUtils.h"

UDustyMovementComponent::UDustyMovementComponent()
{
	bOrientRotationToMovement = true; // Character moves in the direction of input...	
	RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate
}

void UDustyMovementComponent::StopMovement()
{
	Acceleration = FVector::ZeroVector;
	Velocity = FVector::ZeroVector;
}

void UDustyMovementComponent::SetVelocity(FVector _NewVelocity)
{
	Velocity = _NewVelocity;
}


void UDustyMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();
	m_DustyCharacterOwner = Cast<ADustyCharacter>(GetOwner());
}


bool UDustyMovementComponent::IsCustomMovementMode(ECustomMovementMode Mode) const
{
	return MovementMode == MOVE_Custom && CustomMovementMode == Mode;
}

bool UDustyMovementComponent::IsMovementMode(EMovementMode InMovementMode) const
{
	return InMovementMode == MovementMode;
}


void UDustyMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);
}

float UDustyMovementComponent::GetMaxSpeed() const
{
	if (IsMovementMode(MOVE_Walking)) return MaxWalkSpeed;

	if (MovementMode != MOVE_Custom) return Super::GetMaxSpeed();

	return -1.f;
}

float UDustyMovementComponent::GetMaxBrakingDeceleration() const
{
	if (MovementMode != MOVE_Custom) return Super::GetMaxBrakingDeceleration();

	return -1.f;
}

float UDustyMovementComponent::GetMaxAcceleration() const
{
	if (MovementMode != MOVE_Custom) return Super::GetMaxAcceleration();

	switch (CustomMovementMode)
	{
	case CMOVE_PoleJump:
		return Super::GetMaxAcceleration();
	default:
		return -1.f;
	}
}

bool UDustyMovementComponent::CanStartClimbing()
{
	return false;
}

void UDustyMovementComponent::StartClimbing()
{
	//DustyUtils::PrintMessage("Function to start climbing with Dusty may need other implementation", FColor::Yellow, 100.f);
}

void UDustyMovementComponent::CalcVelocity(float DeltaTime, float Friction, bool bFluid, float BrakingDeceleration)
{
	Super::CalcVelocity(DeltaTime, Friction, bFluid, BrakingDeceleration);
}

#pragma endregion