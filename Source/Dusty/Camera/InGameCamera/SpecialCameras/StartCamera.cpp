#include "StartCamera.h"

#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Dusty/Camera/DustyCameraActor.h"
#include "Dusty/Camera/DustyCameraManager.h"
#include "Dusty/Character/DustyCharacter.h"
#include "GameFramework/SpringArmComponent.h"

void AStartCamera::Init(bool _IsFollowUpCamera)
{
	if (RefChecker())
	{
		// set camera at location
		CameraManager->CameraActor->SetActorTransform(m_TargetArrow->GetComponentTransform());
		
		// set camera view on camera actor
		CameraManager->GetOwningPlayerController()->SetViewTargetWithBlend(
			CameraManager->CameraActor, 0.f, VTBlend_Linear);

		// set exit values
		Player->CameraBoom->TargetArmLength = CameraManager->m_BaseSpringArmLength;
		Player->CameraBoom->SocketOffset = CameraManager->m_BaseSpringArmSocketOffset;
		
		FRotator const ExitRotation = FRotator(m_ExitPitchValue, Player->GetActorRotation().Yaw, 0.f);
		CameraManager->GetOwningPlayerController()->SetControlRotation(ExitRotation);
	}
}

void AStartCamera::Exit()
{
	if (RefChecker())
	{
		m_ReachedLocation = false;
		m_ReachedRotation = false;
		
		m_TargetLocation = Player->FollowCamera->GetComponentLocation();
		m_TargetRotation = Player->FollowCamera->GetComponentRotation();

		SetBlendSpeeds();
		
		// disable player's input
		Player->SetInput(false);
		
	}
}

void AStartCamera::Update(float DeltaTime)
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
		CameraManager->GetOwningPlayerController()->SetViewTargetWithBlend(
		CameraManager->ControlledCharacter, 0.f, VTBlend_Linear);
			
		CameraManager->CameraOverrider = nullptr;

		// enable player's input
		Player->SetInput(true);
	}
}

void AStartCamera::SetBlendSpeeds()
{
	m_MoveSpeed = FVector::Dist(m_CurrentLocation, m_TargetLocation) / m_TransitionTime;
	m_RotateSpeed = DustyUtils::GetAngularDistance(m_CurrentRotation, m_TargetRotation) / m_TransitionTime;
}