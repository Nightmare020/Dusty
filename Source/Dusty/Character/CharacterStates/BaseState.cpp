#include "BaseState.h"

#include "Engine/GameInstance.h"
#include <Dusty/InteractiveObject/Key/KeyController.h>
#include "ActionsFilter.h"
#include "Dusty/Camera/DustyCameraManager.h"
#include "../DustyMovementComponent.h"
#include "../../Utils/DustyUtils.h"
#include "..\..\InteractiveObject\Key\KeyController.h"
#include "../../Movement/PoleJumpConstraintActor.h"
#include "../../UI/HUD/HUDSubsystem.h"
#include "../../InteractiveObject/Interactable/Interactable.h"
#include "../../InteractiveObject/Interactable/PoleElevator.h"
#include "../../Cinematics/CinematicsDataAsset.h"
#include "../../Cinematics/CinematicsSubsystem.h"
#include "Components/PoseableMeshComponent.h"
#include "../../InteractiveObject/Key/PlanetsKeyController.h"
#include "../../InteractiveObject/Key/PoleKeyActor.h"
#include "../../UI/HUD/PlayerHUDWidget.h"
#include "Dusty/Animations/DustyAnimInstance.h"

#pragma region BaseState

UBaseState::UBaseState()
{
}

UBaseState::~UBaseState()
{
}

void UBaseState::Update(float _DeltaTime)
{
}

#pragma endregion

#pragma region GroundMovement

void UGroundMovementState::Init()
{
	Super::Init();
	CanUpdateTick = true;
	GetOwner()->GetDustyMovementComponent()->bOrientRotationToMovement = true;
	if (!GetOwner()->CameraManager)
	{
		GetOwner()->SetCameraManagerIfNotInit();
	}
	GetOwner()->HideCrosshair();
	GetOwner()->CameraManager->InitBasicLocomotionCamera();
	GetOwner()->SetRunSpeed();

	//Placeholder
	GetOwner()->PoleSkeletal->SetHiddenInGame(false);

	/*if(GetOwner()->m_trail2)
	{
		GetOwner()->m_trail2->Deactivate();
	}*/
	
}

void UGroundMovementState::Update(float _DeltaTime)
{
	Super::Update(_DeltaTime);
	GetOwner()->UpdateMovement(_DeltaTime);

	if (!GetOwner()->CameraManager->IsOverrided())
	{
		GetOwner()->UpdateLookCamera();
	}
	GetOwner()->CameraManager->UpdateBasicLocomotionCamera(_DeltaTime);
}

void UGroundMovementState::Exit()
{
	Super::Exit(); 
	GetOwner()->CameraManager->ExitBasicLocomotionCamera();
}

#pragma endregion

#pragma region JumpingState

void UJumpingState::Init()
{
	Super::Init();
	CanUpdateTick = true;
	if (GetOwner()->MovementInput.Length() == 0)
	{
		GetOwner()->m_DustyPlayerAnimInstance->SetIsStaticJump(true);
	}

	GetOwner()->GetDustyMovementComponent()->bOrientRotationToMovement = false;
	GetOwner()->Jump();
	GetOwner()->CameraManager->InitBasicLocomotionCamera();
}

void UJumpingState::Update(float _DeltaTime)
{
	Super::Update(_DeltaTime);
	GetOwner()->UpdateLookCamera();
	GetOwner()->CameraManager->UpdateBasicLocomotionCamera(_DeltaTime);
	GetOwner()->UpdateMovement(_DeltaTime);
	
}

void UJumpingState::Exit()
{
	Super::Exit();

	GetOwner()->StopJumping();
	GetOwner()->CameraManager->ExitBasicLocomotionCamera();

}

#pragma endregion

#pragma region AimingState

void UAimingState::Init()
{
	Super::Init();
	CanUpdateTick = true;
	GetOwner()->InitAiming();
	GetOwner()->CameraManager->InitAimCamera();
}

void UAimingState::Update(float _DeltaTime)
{
	Super::Update(_DeltaTime);
	GetOwner()->UpdateAiming(_DeltaTime);
	GetOwner()->UpdateAimingCamera();
	GetOwner()->UpdateMovement(_DeltaTime);
	GetOwner()->CameraManager->UpdateAimCamera(_DeltaTime);
}

void UAimingState::Exit()
{
	Super::Exit();
	GetOwner()->ExitAiming();
	GetOwner()->CameraManager->ExitAimCamera();

	GetOwner()->GetWorld()->GetTimerManager().SetTimer(GetOwner()->AimCooldownHandle, GetOwner(), &ADustyCharacter::ResetCooldown, GetOwner()->AimCooldown, false);

	FTimerHandle ResetAimHandle;

	GetWorld()->GetTimerManager().SetTimer(ResetAimHandle, GetOwner(), &ADustyCharacter::ResetCanAim, 0.5, false);

}

#pragma endregion

#pragma region ClimbingState

void UPoleElevatorState::Init()
{
	Super::Init();
	GetOwner()->ExtendBufferForDoubleJump = false;
	CanUpdateTick = true;
	GetOwner()->ResetMovementInput();
	GetOwner()->CameraManager->InitPoleJumpingCamera();
	
}

void UPoleElevatorState::Update(float _DeltaTime)
{
	Super::Update(_DeltaTime);
	if (Extended)
	{
		GetOwner()->UpdatePoleMovement();

	}
	GetOwner()->UpdateLookCamera();
	GetOwner()->CameraManager->UpdatePoleJumpingCamera(_DeltaTime);
}

void UPoleElevatorState::Exit()
{
	GetOwner()->ExitPoleElevator();
	GetOwner()->CameraManager->ExitBasicLocomotionCamera();
}

#pragma endregion

#pragma region PoleJumpingState

void UPoleJumpingState::Init()
{
	Super::Init();
	CanUpdateTick = true;
	GetOwner()->ResetMovementInput();
	GetOwner()->CameraManager->InitPoleJumpingCamera();

	/*if(GetOwner()->m_trail2)
	{
		GetOwner()->m_trail2->Activate();
	}*/
}

void UPoleJumpingState::Update(float _DeltaTime)
{
	//GetOwner()->TiltDusty(_DeltaTime);
	GetOwner()->UpdatePoleMovement();
	GetOwner()->UpdateLookCamera();
	GetOwner()->CameraManager->UpdatePoleJumpingCamera(_DeltaTime);
}

void UPoleJumpingState::Exit()
{
	Super::Exit();
	GetOwner()->DustyMovementComponent->SetMovementMode(EMovementMode::MOVE_Walking);
	GetOwner()->CameraManager->ExitPoleJumpingCamera();

	/*if(GetOwner()->m_trail2)
	{
		GetOwner()->m_trail2->Deactivate();
	}*/
}

void UPoleJumpingState::SetPoleJumpValues(PoleJumpValuesStruct* _PoleJumpValues)
{
	m_PoleJumpValues = _PoleJumpValues;
}

#pragma endregion

#pragma region FallingState

void UFallingState::Init()
{
	Super::Init();
	CanUpdateTick = true;
	InitialHeight = GetOwner()->GetActorLocation().Z;
	GetOwner()->HideCrosshair();
}

void UFallingState::Update(float _DeltaTime)
{
	Super::Update(_DeltaTime);
	GetOwner()->UpdateLookCamera();
	if (!GetOwner()->m_DustyPlayerAnimInstance->GetIsStaticJump())
	{
		GetOwner()->UpdateMovement(_DeltaTime);
	}
	GetOwner()->CheckFailState();
}

void UFallingState::Exit()
{
	Super::Exit();
	GetOwner()->m_DustyPlayerAnimInstance->SetIsStaticJump(false);
}

#pragma endregion

#pragma region PoleKeyState

void UPoleKeyState::Init()
{
	Super::Init();

	GetOwner()->ExtendBufferForDoubleJump = false;

	CanUpdateTick = true;
	GetOwner()->ResetMovementInput();
	DustyUtils::HideHudWithNoFade(GetWorld(), EHudCanvas::SingleButtonPrompt);
}

void UPoleKeyState::Update(float _DeltaTime)
{
	Super::Update(_DeltaTime);

	if (m_Interactable)
	{
		m_Interactable->Update(_DeltaTime, GetOwner()->MovementInput);
	}
	GetOwner()->UpdateLookCamera();
}

void UPoleKeyState::Exit()
{
	Super::Exit();


}

void UPoleKeyState::SetupPoleKeyState(APoleKeyActor* _Interactable)
{
	m_Interactable = _Interactable;
	if (m_Interactable)
	{
		m_Interactable->SetActorTickEnabled(true);
	}
}

void UPoleKeyState::PoleKeyInserted()
{
	if (m_Interactable)
	{
		m_Interactable->OnPoleInserted();
	}
}

void UPoleKeyState::PoleKeyRemoved()
{
	if (m_Interactable)
	{
		m_Interactable->OnPoleRemoved();
		m_Interactable = nullptr;
	}
}

#pragma endregion

#pragma region PlayingAnimationState

void UAutoMovementState::Init()
{
	Super::Init();
	CanUpdateTick = true;

	GetOwner()->DustyMovementComponent->StopMovement();

	//This is the direction we want to move dusty
	FVector AutoMovingDirection = m_DesiredLocation - GetOwner()->GetActorLocation();
	AutoMovingDirection.Z = 0;
	GetOwner()->AutoMovingDirection = AutoMovingDirection;

	//And we save how far we are from that point
	m_LastDistanceDifference = AutoMovingDirection.Size();
	GetOwner()->CameraManager->InitBasicLocomotionCamera();
	GetOwner()->SetRunSpeed();

}

void UAutoMovementState::Update(float _DeltaTime)
{
	Super::Update(_DeltaTime);

	//We see how far we are from the destination
	FVector Distance = m_DesiredLocation - GetOwner()->GetActorLocation();
	Distance.Z = 0;

	float CurrentPositionDifference = Distance.Size();


	if(CurrentPositionDifference < 0.01f)
	{
		//Place dusty exactly where we want it to be
		GetOwner()->SetActorLocation(m_DesiredLocation);

		//And set to start rotating
		m_AutoRotation = true;
		m_InitialRotation = GetOwner()->GetActorRotation();


		//If there is no need for rotation, we exit here
		float RotationDifference = m_InitialRotation.Yaw - m_DesiredRotation.Yaw;
		if (RotationDifference == 0)
		{
			m_Timer = 0;
			m_AutoRotation = false;
			CallbackDelegate.ExecuteIfBound();
		}

		//We adjust the rotation so it always travels the shortest angle
		if (abs(RotationDifference) > 180)
		{
			m_InitialRotation.Yaw -= 360;
		}

		//We set the time to rotate in function of how much we have to rotate
		m_ActualTimeToRotate = m_BaseTimeToRotate + abs(RotationDifference) / 180 * (m_MaxTimeToRotate - m_BaseTimeToRotate);
	}
	

	//If we reached it, we are rotating 
	if (m_AutoRotation)
	{
		//And this is a simple lerp
		m_Timer += _DeltaTime;
		float NewYaw = FMath::Lerp(m_InitialRotation.Yaw, m_DesiredRotation.Yaw, m_Timer/ m_ActualTimeToRotate);

		GetOwner()->SetActorRotation(FRotator(0, NewYaw, 0));

		if (m_Timer >= m_ActualTimeToRotate)
		{
			GetOwner()->SetActorLocation(m_DesiredLocation);
			//When done, reset all values and execute the function that we receive
			GetOwner()->SetActorRotation(FRotator(0, m_DesiredRotation.Yaw, 0));
			m_Timer = 0;
			m_AutoRotation = false;
			CallbackDelegate.ExecuteIfBound();
		}
	}
	//If we didn't reach it, we keep moving. Dusty will reach when it has moved farther from where he was
	else if ((CurrentPositionDifference - m_LastDistanceDifference) < 1.f)
	{		
		m_LastDistanceDifference = CurrentPositionDifference;
		GetOwner()->UpdateMovement(_DeltaTime);
	}
	//When reached
	else
	{
		//Place dusty exactly where we want it to be
		GetOwner()->SetActorLocation(m_DesiredLocation);

		//And set to start rotating
		m_AutoRotation = true;
		m_InitialRotation = GetOwner()->GetActorRotation();	


		//If there is no need for rotation, we exit here
		float RotationDifference = m_InitialRotation.Yaw - m_DesiredRotation.Yaw; 

		if (FMath::Abs(RotationDifference) < 0.1f)
		{
			m_Timer = 0;
			m_AutoRotation = false;
			CallbackDelegate.ExecuteIfBound();
		}

		//We adjust the rotation so it always travels the shortest angle
		if (abs(RotationDifference) > 180)
		{
			m_DesiredRotation.Yaw -= 360;
		}

		//We set the time to rotate in function of how much we have to rotate
		m_ActualTimeToRotate = m_BaseTimeToRotate + abs(RotationDifference) / 180 * (m_MaxTimeToRotate - m_BaseTimeToRotate);
	}
	GetOwner()->UpdateLookCamera();

}

void UAutoMovementState::Exit()
{
	Super::Exit();
	GetOwner()->CameraManager->ExitBasicLocomotionCamera();
}

void UAutoMovementState::SetAutoMovementValues(FVector _DesiredLocation, FRotator _DesiredRotation)
{
	m_DesiredLocation = _DesiredLocation;
	m_DesiredRotation = _DesiredRotation;
}

#pragma endregion

#pragma region BookState

void UBookState::Init()
{
	Super::Init(); 
	GetOwner()->DustyMovementComponent->StopMovement();
	GetOwner()->DustyMovementComponent->DisableMovement();

	SingleButtonPromptWasShown = DustyUtils::IsHudVisible(GetWorld(), EHudCanvas::SingleButtonPrompt);
	if(SingleButtonPromptWasShown)
	{
		DustyUtils::HideHud(GetWorld(), EHudCanvas::SingleButtonPrompt);
	}

	DoubleButtonPromptWasShown = DustyUtils::IsHudVisible(GetWorld(), EHudCanvas::DoubleButtonPrompt);
	if (DoubleButtonPromptWasShown)
	{
		DustyUtils::HideDoublePromptHud(GetWorld());
	}

	if (OriginState == UPoleKeyState::StaticClass())
	{
		DustyUtils::HideHudWithNoFade(GetWorld(), EHudCanvas::DiscButtons);
	}


	GetOwner()->SetInput(false); // block all input on player controller until timer finishes
	GetOwner()->CameraManager->StartCameraFade(0, 1, GetOwner()->BookFadeDuration, FLinearColor::Black, false, true);
	GetWorld()->GetTimerManager().SetTimer(GetOwner()->FadeTimeHandle, GetOwner(), &ADustyCharacter::ToggleBook, GetOwner()->BookFadeDuration, false);
	CanUpdateTick = true;
	GetOwner()->ResetMovementInput();
}

void UBookState::Update(float _DeltaTime)
{
	Super::Update(_DeltaTime);
}

void UBookState::Exit()
{
	Super::Exit();

	GetOwner()->DustyMovementComponent->StopMovement();
	GetOwner()->DustyMovementComponent->DisableMovement();

	if (OriginState == UPoleKeyState::StaticClass())
	{
		DustyUtils::ShowHud(GetWorld(), EHudCanvas::DiscButtons);
	}
	/*if (SingleButtonPromptWasShown)
	{
		DustyUtils::ShowHud(GetWorld(), EHudCanvas::SingleButtonPrompt, -1);
	}*/
	if (DoubleButtonPromptWasShown)
	{
		DustyUtils::ShowDoublePromptHud(GetWorld());
	}

	GetOwner()->SetInput(false); // block all input on player controller until timer finishes
	GetOwner()->CameraManager->StartCameraFade(0, 1, GetOwner()->BookFadeDuration, FLinearColor::Black, false, true);
	GetWorld()->GetTimerManager().SetTimer(GetOwner()->FadeTimeHandle, GetOwner(), &ADustyCharacter::ToggleBook, GetOwner()->BookFadeDuration, false);
}

#pragma endregion

#pragma region CinematicState

void UCinematicState::Init()
{
	Super::Init();

	GetOwner()->ResetMovementInput();
	GetOwner()->SetActorTickEnabled(false);

	m_DustyController = Cast<APlayerController>(GetOwner()->GetController());

	// Disable player input		
	if (m_DustyController)
	{
		m_DustyController->SetIgnoreMoveInput(true);
		m_DustyController->SetIgnoreLookInput(true);
	}
}

void UCinematicState::Update(float _DeltaTime)
{
	Super::Update(_DeltaTime);
}

void UCinematicState::Exit()
{
	Super::Exit();

	// Get HUD Subsystem
	UHUDSubsystem* hudSubsystem = GetOwner()->GetGameInstance()->GetSubsystem<UHUDSubsystem>();

	// Hide the skip cinematic canvas if it's visible
	if (GetOwner()->GetHUDWidget()->IsSkipCinematicCanvasVisible())
	{
		hudSubsystem->ShowSkipCinematicMessage();
	}

	GetOwner()->SetActorTickEnabled(true);

	// Re-enable player input
	m_DustyController->SetIgnoreMoveInput(false);
	m_DustyController->SetIgnoreLookInput(false);
}

#pragma endregion

#pragma region CameraAnimationState

void UCameraAnimationState::Init()
{
	Super::Init();
	CanUpdateTick = false;
	GetOwner()->ResetMovementInput();
}

void UCameraAnimationState::Update(float _DeltaTime)
{
	Super::Update(_DeltaTime);
}

void UCameraAnimationState::Exit()
{
	Super::Exit();
}

#pragma endregion

#pragma region MainMenuState

void UMainMenuState::Init()
{
	Super::Init();

	m_DustyController = Cast<APlayerController>(GetOwner()->GetController());

	// Disable player input		
	if (m_DustyController)
	{
		m_DustyController->SetIgnoreMoveInput(true);
		m_DustyController->SetIgnoreLookInput(true);
	}
}

void UMainMenuState::Update(float _DeltaTime)
{
	Super::Update(_DeltaTime);
}

void UMainMenuState::Exit()
{
	Super::Exit();

	// Re-enable player input
	m_DustyController->SetIgnoreMoveInput(false);
	m_DustyController->SetIgnoreLookInput(false);
}

#pragma endregion

#pragma region BenchState

void UBenchState::Init()
{
	Super::Init();
	GetOwner()->ResetMovementInput();
	GetOwner()->ExtendBufferForDoubleJump = false;

	GetOwner()->SetActorTickEnabled(false);

	m_DustyController = Cast<APlayerController>(GetOwner()->GetController());

	// Disable player input		
	if (m_DustyController)
	{
		m_DustyController->SetIgnoreMoveInput(true);
		m_DustyController->SetIgnoreLookInput(true);
	}
}

void UBenchState::Update(float _DeltaTime)
{
	Super::Update(_DeltaTime);
}

void UBenchState::Exit()
{
	Super::Exit();

	// Get HUD Subsystem
	UHUDSubsystem* hudSubsystem = GetOwner()->GetGameInstance()->GetSubsystem<UHUDSubsystem>();
	
	// Hide the skip cinematic canvas if it's visible
	if (GetOwner()->GetHUDWidget()->IsSkipCinematicCanvasVisible())
	{
		hudSubsystem->ShowSkipCinematicMessage();
	}

	GetOwner()->SetActorTickEnabled(true);

	// Re-enable player input
	m_DustyController->SetIgnoreMoveInput(false);
	m_DustyController->SetIgnoreLookInput(false);


	GetOwner()->PlayAnimMontage(GetOwner()->StandUpAnimation);
}

#pragma endregion

#pragma region OpeningState

void UOpeningState::Init()
{
	Super::Init();

	GetOwner()->SetActorTickEnabled(false);

	m_DustyController = Cast<APlayerController>(GetOwner()->GetController());
}

void UOpeningState::Update(float _DeltaTime)
{
	Super::Update(_DeltaTime);
}

void UOpeningState::Exit()
{
	Super::Exit();

	GetOwner()->SetActorTickEnabled(true);
}

#pragma endregion

#pragma region SittingState
void USittingState::Init()
{
	Super::Init();
	m_DustyController = Cast<APlayerController>(GetOwner()->GetController());
	CanUpdateTick = true;
	GetOwner()->ExtendBufferForDoubleJump = false;

	if (m_DustyController)
	{
		m_DustyController->SetIgnoreMoveInput(true);
	}
}

void USittingState::Update(float _DeltaTime)
{
	Super::Update(_DeltaTime);
	if (m_DustyController && !m_standingUp)
	{
		if (m_DustyController->WasInputKeyJustPressed(EKeys::AnyKey))// || m_DustyController->IsInputKeyDown(EKeys::AnyKey))
		{
			m_standingUp = true;
			GetOwner()->PlayAnimMontage(GetOwner()->StandUpAnimation);
		}
	}
	GetOwner()->UpdateLookCamera();
}

void USittingState::Exit()
{
	Super::Exit();
	m_standingUp = false;

	if (m_DustyController)
	{
		m_DustyController->SetIgnoreMoveInput(false);
	}
}
#pragma endregion