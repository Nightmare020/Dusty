#include "ForceLookCamera.h"

#include "Components/ArrowComponent.h"
#include "Dusty/Camera/DustyCameraManager.h"
#include "Dusty/Character/DustyCharacter.h"

AForceLookCamera::AForceLookCamera()
{
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("Root"));
	m_TargetArrow = CreateDefaultSubobject<UArrowComponent>("TargetArrow");
	m_TargetArrow->SetupAttachment(RootComponent);
}


void AForceLookCamera::Update(float DeltaTime)
{
	if(RefChecker())
	{
		if ( APlayerController* PlayerController = CameraManager->GetOwningPlayerController())
		{
			FRotator CurrentRotation = PlayerController->GetControlRotation();
			const FRotator TargetRotation = m_TargetArrow->GetComponentRotation();

			ADustyCameraManager::CameraLerp(DeltaTime, CurrentRotation, TargetRotation, m_RotateSpeed);
			PlayerController->SetControlRotation(CurrentRotation);
		}
	}
}

void AForceLookCamera::Init(bool _IsFollowUpCamera)
{
	if(RefChecker() && IsActive)
	{
		CameraManager->CameraOverrider = this;
		Player->SetInput(false);
	}
}

void AForceLookCamera::Exit()
{
	if(RefChecker())
	{
		IsActive = false;
		Player->SetInput(true);
		CameraManager->CameraOverrider = nullptr;
		
		if (NextCamera)
		{
			// since its in the player camera component dont call follow up even tho it is
			NextCamera->Init(false);
		}
	}
}