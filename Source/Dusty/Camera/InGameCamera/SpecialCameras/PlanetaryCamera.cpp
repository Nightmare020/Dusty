#include "PlanetaryCamera.h"

#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Dusty/Camera/DustyCameraActor.h"
#include "Dusty/Camera/DustyCameraManager.h"
#include "Dusty/Character/DustyCharacter.h"
#include "Dusty/InteractiveObject/Key/PlanetsKeyController.h"


APlanetaryCamera::APlanetaryCamera()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("Root"));
	
	m_TargetArrow = CreateDefaultSubobject<UArrowComponent>("TargetLocation");
	m_TargetArrow->SetupAttachment(RootComponent);
}

void APlanetaryCamera::Init(bool _IsFollowUpCamera)
{
	if (RefChecker())
	{
		CameraManager->CameraOverrider = this;
		m_IsFollowUpCamera = _IsFollowUpCamera;
		m_PlanetsKeyController->SetCamera(this);
		m_Current = 0;
		FadeControl();
	}
}


void APlanetaryCamera::Exit()
{
	if (RefChecker())
	{
		if (CameraManager->CameraOverrider == this)
		{
			m_Current = 2;
			FadeControl();
		}
	}
}


void APlanetaryCamera::FadeControl()
{
	switch (m_Current)
	{
	case 0: // start camera fade in
		
		// block players input
		Player->SetInput(false);
		
		GetWorld()->GetTimerManager().SetTimer(m_TimeHandle, this, &APlanetaryCamera::FadeControl, m_FadeTime, false);
		CameraManager->StartCameraFade(0, 1, m_FadeTime, FLinearColor::Black, false, true);
		break;
		
	case 1: // fade 1 finished, TP camera and start fade out

		if (m_PlanetsKeyController)
		{
			m_PlanetsKeyController->CameraInFade(true);
		}
		// store and set new fov value
		m_CurrentFov = CameraManager->CameraActor->GetCameraComponent()->FieldOfView;
		CameraManager->CameraActor->GetCameraComponent()->FieldOfView = m_FOV;

		// place camera
		CameraManager->CameraActor->SetActorLocation(m_TargetArrow->GetComponentLocation());
		CameraManager->CameraActor->SetActorRotation(m_TargetArrow->GetComponentRotation());
		
		CameraManager->GetOwningPlayerController()->SetViewTargetWithBlend(CameraManager->CameraActor, 0.f, VTBlend_Linear);
		
		CameraManager->StartCameraFade(1, 0, m_FadeTime, FLinearColor::Black, false, true);

		// timer to toggle input when finished
		m_CurrentDesiredInput = true;
		
		GetWorld()->GetTimerManager().SetTimer(m_TimeHandle, this, &APlanetaryCamera::Reached, m_FadeTime, false);
		
		// c++ sometimes is funny isn't it
		{
			// set camera component location for later exit procedures
			FRotator const ExitRotation = FRotator(m_ExitPitchValue, Player->GetActorRotation().Yaw, 0.f);
			Player->CameraManager->GetOwningPlayerController()->SetControlRotation(ExitRotation);
		}
		break;

		
	case 2: // TP finished && toggle actors activated start now exit camera fade in
		
		// block players input
		m_CurrentDesiredInput = false;
		Player->SetInput(false);
		
		GetWorld()->GetTimerManager().SetTimer(m_TimeHandle, this, &APlanetaryCamera::FadeControl, m_FadeTime, false);
		CameraManager->StartCameraFade(0, 1, m_FadeTime, FLinearColor::Black, false, true);

		break;
		
	case 3: // exit camera fade in finished TP camera back to player and start exit camera fade out

		if (m_PlanetsKeyController)
		{
			m_PlanetsKeyController->CameraInFade(false);
		}
		CameraManager->StartCameraFade(1, 0, m_FadeTime, FLinearColor::Black, false, true);

		// Set back initial FOV
		CameraManager->CameraActor->GetCameraComponent()->FieldOfView = m_CurrentFov;
		
		// timer to toggle input when finished
		m_CurrentDesiredInput = true;
		GetWorld()->GetTimerManager().SetTimer(m_TimeHandle, this, &APlanetaryCamera::Exited, m_FadeTime, false);
		
		// set camera component as camera again
		CameraManager->GetOwningPlayerController()->SetViewTargetWithBlend(CameraManager->ControlledCharacter, 0.f, VTBlend_Linear);
		
		break;
		
	default:
		break;
	}
	
	++m_Current;
}

void APlanetaryCamera::Reached()
{
	if (RefChecker())
	{
		if (m_PlanetsKeyController)
		{
			m_PlanetsKeyController->CameraSetted();
		}
		Player->SetInput(true);
	}
}

void APlanetaryCamera::Exited()
{
	if (RefChecker())
	{
		CameraManager->CameraOverrider = nullptr;
		Player->SetInput(true);
	}
}