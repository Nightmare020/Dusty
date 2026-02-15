#include "CameraFocusToEvent.h"

#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Dusty/Camera/DustyCameraActor.h"
#include "Dusty/Camera/DustyCameraManager.h"
#include "Dusty/Camera/InGameCamera/CameraModifiers/CM_SmoothMovement.h"
#include "Dusty/Character/DustyCharacter.h"
#include "Dusty/Character/CharacterStates/ActionsFilter.h"
#include "Dusty/Character/CharacterStates/BaseState.h"
#include "Dusty/Utils/DustyUtils.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"


ACameraFocusToEvent::ACameraFocusToEvent()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("Root"));
	
	TargetArrow = CreateDefaultSubobject<UArrowComponent>("POILocation");
	TargetArrow->SetupAttachment(RootComponent);
}


void ACameraFocusToEvent::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);
}


void ACameraFocusToEvent::SetBlendSpeeds()
{
	m_MoveSpeed = FVector::Dist(m_CurrentLocation, m_TargetLocation) / m_TransitionTime;
	m_RotateSpeed = DustyUtils::GetAngularDistance(m_CurrentRotation, m_TargetRotation) / m_TransitionTime;
}

void ACameraFocusToEvent::Update(float DeltaTime)
{
	Super::Update(DeltaTime);

	// todo: Fix this please
	if (m_Mode == ECameraState::Exiting && m_BugFix)
	{
		m_BugFix = false;
		m_TargetRotation = CameraManager->CameraComponent->GetComponentRotation();
		m_TargetLocation = CameraManager->CameraComponent->GetComponentLocation();
		SetBlendSpeeds();
	}
	
	if (m_Mode == ECameraState::Reached)
	{
		m_Timer -= DeltaTime;
		if (m_Timer <= 0.f)
		{
			Exit(); // start exiting procedures
		}
	}
	else
	{
		if (!m_ReachedRotation)
		{
			m_ReachedRotation = ADustyCameraManager::CameraLerp(DeltaTime, m_CurrentRotation, m_TargetRotation, m_RotateSpeed);
			CameraManager->CameraActor->SetActorRotation(m_CurrentRotation);
		}
		
		if (!m_ReachedLocation)
		{
			m_ReachedLocation = ADustyCameraManager::CameraLerp(DeltaTime, m_CurrentLocation, m_TargetLocation, m_MoveSpeed);
			CameraManager->CameraActor->SetActorLocation(m_CurrentLocation);
		}

		if (m_ReachedRotation && m_ReachedLocation)
		{
			if (m_Mode == ECameraState::Entering)
			{
				// start wait mode on camera
				m_Mode = ECameraState::Reached;
				m_Timer = m_FocusTime;
				
				Reached(); // call for reach action in case there is anything needed
			}
			else // exiting camera moder
			{
				CameraManager->GetOwningPlayerController()->SetViewTargetWithBlend(
				CameraManager->ControlledCharacter, 0.f, VTBlend_Linear);
				
				CameraManager->CameraOverrider = nullptr;
				
				Super::Exit();

				// launch exit actions if any needed
				Exited();
			}
		}
	}
}


void ACameraFocusToEvent::Init(bool _IsFollowUpCamera)
{
	if (IsActive)
	{
		// get references to player and camera manager
		if(RefChecker())
		{
			// if it is follow up camera then the starting position is the current camera actor position
			if (!_IsFollowUpCamera)
			{
				// set targets
				m_CurrentLocation = CameraManager->CameraComponent->GetComponentLocation();
				m_CurrentRotation = CameraManager->CameraComponent->GetComponentRotation();

				CameraManager->CameraActor->SetActorLocation(m_CurrentLocation);
				CameraManager->CameraActor->SetActorRotation(m_CurrentRotation);
			}
			else
			{
				m_CurrentLocation = CameraManager->CameraActor->GetActorLocation();
				m_CurrentRotation = CameraManager->CameraActor->GetActorRotation();
			}
			
			//m_AssociatedToggleActorActivateValue = _AssociatedToggleActorActivateValue;
			m_Mode = ECameraState::Entering;;
		
			CameraManager->GetOwningPlayerController()->SetViewTargetWithBlend(
						CameraManager->CameraActor, 0.f, VTBlend_Linear);
		
			m_TargetLocation = TargetArrow->GetComponentLocation();
			m_TargetRotation = TargetArrow->GetComponentRotation();
			SetBlendSpeeds();

			// set camera modes
			CameraManager->CameraOverrider = this;
		
			m_ReachedLocation = false;
			m_ReachedRotation = false;

			if(m_ModifyCurrentState)
			{
				// change dusty state to camera state
				Player->ActionsFilterComponent->SetCurrentState(UCameraAnimationState::StaticClass());
			}
			if (m_BlockPlayersInput)
			{
				// block input for the whole process
				Player->SetInput(false);
			}

			// disable active modifier and activate ground movement modifier
			CameraManager->SetModifier(false);
			CameraManager->SetModifier(UCM_SmoothMovement::StaticClass(), true);
		}
	}
}


void ACameraFocusToEvent::Exit()
{
	if (IsActive)
	{
		if(RefChecker())
		{
			Super::Exit();
			if (NextCamera)
			{
				
				CameraManager->CameraOverrider = NextCamera;
				NextCamera->Init(true);
			}
			else
			{
				m_Mode = ECameraState::Exiting;
				m_BugFix = true;

				m_ReachedLocation = false;
				m_ReachedRotation = false;
		
				// set spring arm value and socket offset to base value
				Player->CameraBoom->TargetArmLength = CameraManager->m_BaseSpringArmLength;
				Player->CameraBoom->SocketOffset = CameraManager->m_BaseSpringArmSocketOffset;
			
				// set camera component location for later exit procedures
				FRotator const ExitRotation = FRotator(m_ExitPitchValue, Player->GetActorRotation().Yaw, 0.f);
				CameraManager->GetOwningPlayerController()->SetControlRotation(ExitRotation);
	
				// set targets
				m_CurrentLocation = CameraManager->CameraActor->GetActorLocation();
				m_CurrentRotation = CameraManager->CameraActor->GetActorRotation();
			}
		}
	}
}

void ACameraFocusToEvent::Reached()
{
	Super::Reached();
}

void ACameraFocusToEvent::Exited()
{
	Super::Exited();

	if(m_ModifyCurrentState)
	{
		// change dusty state to ground movement state
		Player->ActionsFilterComponent->SetCurrentState(UGroundMovementState::StaticClass());
		Player->SetInput(false); // reset input
		Player->SetInput(true); // reset input
	}
	if (m_BlockPlayersInput)
	{
		// block input for the whole process
		Player->SetInput(true);
	}
}

float ACameraFocusToEvent::GetTransitionTime() const
{
	return m_TransitionTime;
}
