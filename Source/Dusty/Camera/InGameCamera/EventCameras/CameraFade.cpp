#include "CameraFade.h"

#include "Components/ArrowComponent.h"
#include "Dusty/Camera/DustyCameraActor.h"
#include "Dusty/Camera/DustyCameraManager.h"
#include "Dusty/Character/CharacterStates/ActionsFilter.h"
#include "Dusty/Character/CharacterStates/BaseState.h"
#include "Kismet/GameplayStatics.h"


ACameraFade::ACameraFade()
{
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("Root"));
	
	m_TargetArrow = CreateDefaultSubobject<UArrowComponent>("POILocation");
	m_TargetArrow->SetupAttachment(RootComponent);
}

void ACameraFade::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);
}

void ACameraFade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	m_FocusTimer -= DeltaTime;

	// focus timer finished
	if (m_FocusTimer <= 0.f)
	{
		// call reached control
		FadeControl();
		SetActorTickEnabled(false);
	}
}

void ACameraFade::Init(bool _IsFollowUpCamera)
{
	if (RefChecker())
	{
		if (CameraManager && IsActive)
		{
			Super::Init();
			// block player's capability to receive input till fade in fade out is finished
			Player->SetInput(false);
			
			// set players state to camera animation so it can't move
			Player->ActionsFilterComponent->SetCurrentState(UCameraAnimationState::StaticClass());
		
			// set control value to starting
			m_Current = 0;
			FadeControl();
			m_OneUseOnly = false;
		}
	}
}

void ACameraFade::Reached()
{
	Super::Reached();
}

void ACameraFade::Exited()
{
	Super::Exited();
	if(RefChecker())
	{
		// enable players input again
		Player->SetInput(true);
		
		// reset player state to ground and exit camera
		Player->ActionsFilterComponent->SetCurrentState(UGroundMovementState::StaticClass());
		IsActive = false;
	}
}

void ACameraFade::FadeControl()
{
	switch (m_Current)
	{
	case 0: // start camera fade in
		GetWorld()->GetTimerManager().SetTimer(m_TimeHandle, this, &ACameraFade::FadeControl, m_FadeTime, false);
		CameraManager->StartCameraFade(0, 1, m_FadeTime, FLinearColor::Black, false, true);
		break;
		
	case 1: // fade 1 finished, TP camera and start fade out
		// place camera
		CameraManager->CameraActor->SetActorLocation(m_TargetArrow->GetComponentLocation());
		CameraManager->CameraActor->SetActorRotation(m_TargetArrow->GetComponentRotation());
		
		CameraManager->GetOwningPlayerController()->SetViewTargetWithBlend(CameraManager->CameraActor, 0.f, VTBlend_Linear);

		GetWorld()->GetTimerManager().SetTimer(m_TimeHandle, this, &ACameraFade::FocusControl, m_FadeTime, false);
		CameraManager->StartCameraFade(1, 0, m_FadeTime, FLinearColor::Black, false, true);

		// c++ sometimes is funny isn't it
		{
			// set camera component location for later exit procedures
			FRotator const ExitRotation = FRotator(m_ExitPitchValue, m_ExitYawValue, 0.f);
			Player->CameraManager->GetOwningPlayerController()->SetControlRotation(ExitRotation);
		}
		
		break;
		
	case 2: // TP finished && toggle actors activated start now exit camera fade in

		if (NextCamera)
		{
			Exited();
			NextCamera->Init(true);
		}
		else
		{
			GetWorld()->GetTimerManager().SetTimer(m_TimeHandle, this, &ACameraFade::FadeControl, m_FadeTime, false);
			CameraManager->StartCameraFade(0, 1, m_FadeTime, FLinearColor::Black, false, true);
		}
		break;
		
	case 3: // exit camera fade in finished TP camera back to player and start exit camera fade out
		GetWorld()->GetTimerManager().SetTimer(m_TimeHandle, this, &ACameraFade::Exited, m_FadeTime, false);
		CameraManager->StartCameraFade(1, 0, m_FadeTime, FLinearColor::Black, false, true);
		
		// set camera component as camera again
		CameraManager->GetOwningPlayerController()->SetViewTargetWithBlend(CameraManager->ControlledCharacter, 0.f, VTBlend_Linear);
		
		// call for deletion procedures
		Super::Exit();
		
		break;
		
	default:
		break;
	}
	
	++m_Current;
}

void ACameraFade::FocusControl()
{
	// activate tick so we wait the focus time
	m_FocusTimer = m_FocusTime;

	// do reached logic
	Reached();

	// enable tick for wait focus time
	SetActorTickEnabled(true);
}
