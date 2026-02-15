#include "DustyAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "../Character/CharacterStates/ActionsFilter.h"
#include "../Utils/DustyUtils.h"
#include "../Movement/PoleJumpConstraintActor.h"
#include "../InteractiveObject/Interactable/PoleElevator.h"
#include "InputActionValue.h"
void UDustyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	m_DustyCharacter = Cast<ADustyCharacter>(TryGetPawnOwner());


#if WITH_EDITOR
	m_showDebugs = true;
#else 
	m_showDebugs = false;
#endif

	if (m_DustyCharacter)
	{
		m_DustyMovementComponent = m_DustyCharacter->GetDustyMovementComponent();
	}
}

void UDustyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);


	if (!m_DustyCharacter || !m_DustyMovementComponent) return;

	GetGroundSpeed();
	GetAirSpeed();
	GetShouldMove();
	GetIsFalling();
}

void UDustyAnimInstance::GetGroundSpeed()
{
	m_GroundSpeed = UKismetMathLibrary::VSizeXY(m_DustyCharacter->GetVelocity());
}

void UDustyAnimInstance::GetAirSpeed()
{
	m_AirSpeed = m_DustyCharacter->GetVelocity().Z;
}

void UDustyAnimInstance::GetShouldMove()
{
	m_ShouldMove = (m_DustyMovementComponent->GetCurrentAcceleration().Size() > 0) && (m_GroundSpeed > 5.f) && (!m_IsFalling);
}

void UDustyAnimInstance::GetIsFalling()
{
	m_IsFalling = m_DustyMovementComponent->IsFalling();
}

bool UDustyAnimInstance::GetIsStaticJump()
{
	return m_staticJump;
}

void UDustyAnimInstance::SetIsStaticJump(bool jump)
{
	m_staticJump = jump;
}

bool UDustyAnimInstance::GetForwardPoleElevator()
{
	return m_ForwardPoleJump;
}

void UDustyAnimInstance::SetForwardPoleElevator(bool forward)
{
	m_ForwardPoleJump = forward;
}

bool UDustyAnimInstance::GetInputAfterPoleJump()
{
	return m_InputAfterPoleJump;
}

void UDustyAnimInstance::SetInputAfterPoleJump(bool Input)
{
	m_InputAfterPoleJump = Input;
}


void UDustyAnimInstance::PlayMontage(UAnimMontage* MontageToPlay)
{
	if (MontageToPlay && !IsAnyMontagePlaying())
	{
		Montage_Play(MontageToPlay);
	}
}

void UDustyAnimInstance::CalculateGroundValues()
{
	//Ground values
	if (m_DustyMovementComponent != nullptr && m_DustyMovementComponent->GetOwner() != nullptr)
	{
		m_groundVelocity.X = FVector::DotProduct(m_DustyMovementComponent->GetOwner()->GetActorRightVector(), m_DustyMovementComponent->GetOwner()->GetVelocity());
		m_groundVelocity.Y = FVector::DotProduct(m_DustyMovementComponent->GetOwner()->GetActorForwardVector(), m_DustyMovementComponent->GetOwner()->GetVelocity());
		m_groundVelocityMagnitude = m_groundVelocity.Length();
	}
}

void UDustyAnimInstance::CalculateRotationValues()
{
	if (m_DustyMovementComponent != nullptr && m_DustyMovementComponent->GetOwner() != nullptr)
	{
		float atan2A = atan2(m_DustyMovementComponent->GetOwner()->GetActorForwardVector().Y, m_DustyMovementComponent->GetOwner()->GetActorForwardVector().X);
		float atan2B = atan2(m_desiredPlayerRotation.Y, m_desiredPlayerRotation.X);
		float angle = ((int)(atan2A - atan2B + 180) % 360) - 180;
		m_desiredPlayerAngle = angle * -1;
		m_walkStartAngle = angle;
		
	}

	if (TryGetPawnOwner() != nullptr && TryGetPawnOwner()->Controller != nullptr && m_DustyCharacter != nullptr)
	{
		FVector RightVector = UKismetMathLibrary::GetRightVector(TryGetPawnOwner()->GetControlRotation());
		FVector ForwardVector = UKismetMathLibrary::GetForwardVector(TryGetPawnOwner()->GetControlRotation());

		FVector DesiredVector = (RightVector * m_DustyCharacter->MovementInput.X) + (ForwardVector * m_DustyCharacter->MovementInput.Y);
		DesiredVector = FVector(DesiredVector.X, DesiredVector.Y, 0);
		DesiredVector.Normalize();

		m_desiredPlayerRotation = DesiredVector;
		m_RotationInput = m_DustyCharacter->LookInput;
	}
}

void UDustyAnimInstance::CalculateAimValues()
{
	if (TryGetPawnOwner() != nullptr && TryGetPawnOwner()->Controller != nullptr)
	{
		FVector2D actualDir = FVector2D(
			UKismetMathLibrary::ClampAngle(TryGetPawnOwner()->GetControlRotation().Euler().Z - TryGetPawnOwner()->GetActorRotation().Euler().Z, -28, 28),
			UKismetMathLibrary::ClampAngle(TryGetPawnOwner()->GetControlRotation().Euler().Y, -85, 85)
		);

		FVector2D newDir = actualDir - FVector2D(m_AoHortizontalAim, m_AoVerticalAim);

		float inertiaStrength = 7.f; //Don�t modify
		m_AoAimInertia = FMath::Lerp(m_AoAimInertia, newDir, inertiaStrength * GetWorld()->GetDeltaSeconds());

		m_AoVerticalAim = actualDir.Y;
		m_AoHortizontalAim = m_AoAimInertia.X;

	}
}

void UDustyAnimInstance::CalculateTiltedValues()
{
	if (TryGetPawnOwner() != nullptr && TryGetPawnOwner()->Controller != nullptr)
	{
		FVector2D actualDir = FVector2D(
			UKismetMathLibrary::ClampAngle(TryGetPawnOwner()->GetControlRotation().Euler().Z - TryGetPawnOwner()->GetActorRotation().Euler().Z, -28, 28),
			UKismetMathLibrary::ClampAngle(TryGetPawnOwner()->GetControlRotation().Euler().Y, -85, 85)
		);

		FVector2D newDir = actualDir - FVector2D(m_TitledHortizontal, actualDir.Y);

		float inertiaStrength = 7.f; //Don�t modify
		if (m_DustyCharacter->MovementInput.X > 0.3f || m_DustyCharacter->MovementInput.X < -0.3f)
		{
			m_TiltedInertia = FMath::Lerp(m_TiltedInertia, newDir, inertiaStrength * GetWorld()->GetDeltaSeconds());
		}
		else
		{
			m_TiltedInertia = FMath::Lerp(m_TiltedInertia, FVector2D::Zero(), inertiaStrength * GetWorld()->GetDeltaSeconds());
		}

		m_TitledHortizontal = m_TiltedInertia.X;
	}
}

void UDustyAnimInstance::DeterminePlayerState()
{
	if (m_DustyCharacter != nullptr)
	{
		//There is no other way to do it :)
		TSubclassOf<UBaseState> state = m_DustyCharacter->ActionsFilterComponent->GetCurrentBaseStateClass();

		if (state == UGroundMovementState::StaticClass() || state == UAutoMovementState::StaticClass())
		{
			m_ShouldUseIkRig = true;
			if (m_groundVelocityMagnitude < 10)
			{
				m_CurrentPlayerState = PlayerStates::Neutral;
			}
			else if (m_DustyCharacter->DesiredPlayerSpeed <= m_DustyCharacter->GetDustyMaxWalkSpeed())
			{
				m_CurrentPlayerState = PlayerStates::Walking;
			}
			else
			{
				m_CurrentPlayerState = PlayerStates::Jogging;
			}
		}
		else if (state == UJumpingState::StaticClass())
		{
			m_ShouldUseIkRig = false;
			m_CurrentPlayerState = PlayerStates::Jumping;
		}
		else if (state == UAimingState::StaticClass())
		{
			m_ShouldUseIkRig = true;
			m_CurrentPlayerState = PlayerStates::Aiming;
		}
		else if (state == UPoleElevatorState::StaticClass())
		{
			m_ShouldUseIkRig = false;
			m_CurrentPlayerState = PlayerStates::PoleElevator;
		}
		else if (state == UPoleKeyState::StaticClass())
		{
			m_ShouldUseIkRig = false;
			m_CurrentPlayerState = PlayerStates::PoleKey;
		}
		else if (state == UPoleJumpingState::StaticClass())
		{
			m_ShouldUseIkRig = false;
			m_CurrentPlayerState = PlayerStates::PoleJumping;
		}
		else if (state == UFallingState::StaticClass())
		{
			m_ShouldUseIkRig = false;
			m_CurrentPlayerState = PlayerStates::Falling;
		}
		else if (state == UCinematicState::StaticClass())
		{
			m_ShouldUseIkRig = true;
			m_CurrentPlayerState = PlayerStates::Cinematic;
		}
		else if (state == UMainMenuState::StaticClass())
		{
			m_ShouldUseIkRig = true;
			m_CurrentPlayerState = PlayerStates::MainMenu;
		}
		else if (state == UBenchState::StaticClass())
		{
			m_ShouldUseIkRig = false;
			m_CurrentPlayerState = PlayerStates::Bench;
		}
		else if (state == UOpeningState::StaticClass())
		{
			m_ShouldUseIkRig = false;
			m_CurrentPlayerState = PlayerStates::Opening;
		}
		else if (state == USittingState::StaticClass())
		{
			m_ShouldUseIkRig = false;
			m_CurrentPlayerState = PlayerStates::Sitting;
		}
		else if (state == UBookState::StaticClass())
		{
			m_ShouldUseIkRig = true;
			m_CurrentPlayerState = PlayerStates::Book;
		}
		else
		{
			m_ShouldUseIkRig = false;
			m_CurrentPlayerState = PlayerStates::Neutral;
		}
	}
}

void UDustyAnimInstance::AddAlphaStickCube(float value)
{
	m_BlendStartStickMagnetismAlpha += value;
	m_BlendStartStickMagnetismAlpha = FMath::Clamp(m_BlendStartStickMagnetismAlpha, 0, 1);
}

void UDustyAnimInstance::SetDustyLookAt(FVector Position)
{
	m_positionToLookAt = Position;
}


void UDustyAnimInstance::RemoveDustyLookAt()
{
	m_positionToLookAt = FVector::Zero();
}

void UDustyAnimInstance::AnimNotify_ExtendPoleElevator()
{
	if (m_DustyCharacter && m_DustyCharacter->CurrentPoleElevator)
	{
		m_DustyCharacter->CurrentPoleElevator->SpawnPoleElevator();
	}
}

void UDustyAnimInstance::AnimNotify_PoleKeyInserted()
{
	if (m_DustyCharacter)
	{
		if (UPoleKeyState* State = Cast<UPoleKeyState>(m_DustyCharacter->ActionsFilterComponent->GetCurrentBaseState()))
		{
			State->PoleKeyInserted();
		}
	}
}

void UDustyAnimInstance::AnimNotify_PoleKeyRemoved()
{
	if (m_DustyCharacter)
	{
		if (UPoleKeyState* State = Cast<UPoleKeyState>(*(m_DustyCharacter->ActionsFilterComponent->m_statesInstancesMap.Find(UPoleKeyState::StaticClass()))))
		{
			State->PoleKeyRemoved();
		}
	}
}
void UDustyAnimInstance::AnimNotify_EndInitialJumpInPoleJump()
{
	if (m_DustyCharacter)
	{
		m_DustyCharacter->EndInitialJumpInPoleJump();
	}
}
void UDustyAnimInstance::AnimNotify_StartExtendingBottomPole()
{
	if (m_DustyCharacter)
	{
		m_DustyCharacter->StartExtendingBottomPole();
	}
}

void UDustyAnimInstance::AnimNotify_ResetInputAfterPoleJump()
{
	m_InputAfterPoleJump = false;
}





