#include "DustyCameraManager.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Dusty/Character/DustyMovementComponent.h"
#include "Dusty/Character/CharacterStates/ActionsFilter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Dusty/Camera/DustyCameraActor.h"
#include "Dusty/Utils/DustyUtils.h"
#include "InGameCamera/DynamicCameras/CameraOverrider.h"
#include "InGameCamera/CameraModifiers/CM_Aim.h"
#include "InGameCamera/CameraModifiers/CM_ClimbAutoAdjustment.h"
#include "InGameCamera/CameraModifiers/CM_Pole.h"
#include "InGameCamera/CameraModifiers/CM_SmoothMovement.h"
#include "InGameCamera/CameraModifiers/DustyCameraModifier.h"
#include "Camera/CameraModifier_CameraShake.h"


void ADustyCameraManager::BeginPlay()
{
	Super::BeginPlay();
	DisableModifiers();

	if (ControlledCharacter == nullptr)
	{
		ControlledCharacter = Cast<ADustyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	}
	
	if (ControlledCharacter)
	{
		CameraComponent = ControlledCharacter->FollowCamera;

		// set spring arm default values
		m_BaseSpringArmSocketOffset = ControlledCharacter->CameraBoom->SocketOffset;
		m_BaseSpringArmTargetOffset = ControlledCharacter->CameraBoom->TargetOffset;
		m_BaseSpringArmLength = ControlledCharacter->CameraBoom->TargetArmLength;
	}

	if (CameraActor == nullptr)
	{
		CameraActor = GetWorld()->SpawnActor<ADustyCameraActor>();
	}
	
	for (UCameraModifier* Modifier : ModifierList)
	{
		if (UDustyCameraModifier* CameraModifier = Cast<UDustyCameraModifier>(Modifier))
		{
			CameraModifier->Init(this, ControlledCharacter);
		}
	}

	if (m_SpeedCurve)
	{
		// set max curve value for speed adjustments
		m_MaxCurveValue = m_SpeedCurve->GetFloatValue(1.f);
	}

	//Getting initial blur values
	FPostProcessSettings& PostProcessSettings = CameraActor->GetCameraComponent()->PostProcessSettings;
	Aperture = PostProcessSettings.DepthOfFieldFstop;
	MaxAperture = PostProcessSettings.DepthOfFieldMinFstop;
	DiaphragmBlades = PostProcessSettings.DepthOfFieldBladeCount;

	SensorWitdh = PostProcessSettings.DepthOfFieldSensorWidth;
	FocalDistance = PostProcessSettings.DepthOfFieldFocalDistance;
	DepthBlurRadius = PostProcessSettings.DepthOfFieldDepthBlurRadius;
}


void ADustyCameraManager::UpdateCamera(float DeltaTime)
{
	Super::UpdateCamera(DeltaTime);
	if (m_IsOnStartCamera)
	{
		if (!m_IsOnStartCameraWait)
		{
			if (!m_StartCameraReachedRotation)
			{
				m_StartCameraReachedRotation = CameraLerp(DeltaTime, m_StartCameraCurrentRotation, m_StartCameraTargetRotation, m_StartCameraRotateSpeed);
				CameraActor->SetActorRotation(m_StartCameraCurrentRotation);
			}
		
			if (!m_StartCameraReachedLocation)
			{
				m_StartCameraReachedLocation = CameraLerp(DeltaTime, m_StartCameraCurrentLocation, m_StartCameraTargetLocation, m_StartCameraMoveSpeed);
				CameraActor->SetActorLocation(m_StartCameraCurrentLocation);
			}

			if (m_StartCameraReachedRotation && m_StartCameraReachedLocation)
			{
				GetOwningPlayerController()->SetViewTargetWithBlend(ControlledCharacter, 0.f, VTBlend_Linear);
			
				CameraOverrider = nullptr;

				// enable player's input
				ControlledCharacter->SetInput(true);

				m_IsOnStartCamera = false;
			}
		}
	}
	else
	{
		if (CameraOverrider && !CameraOverrider->IsOnWait)
		{
			CameraOverrider->Update(DeltaTime);
		}
	}
}

#pragma region BasicLocomotionCamera

void ADustyCameraManager::InitBasicLocomotionCamera()
{
	if (ControlledCharacter)
	{
		// check if last state wasn't jump or ground
		if (!m_LastState || (!m_LastState->IsA(UJumpingState::StaticClass()) &&  !m_LastState->IsA(UGroundMovementState::StaticClass())))
		{
			ControlledCharacter->FollowCamera->SetFieldOfView(DefaultFOV);
			
			// set current active modifier
			SetModifier(UCM_SmoothMovement::StaticClass(), true);
		}
	}
}

void ADustyCameraManager::ExitBasicLocomotionCamera()
{
	m_LastState = ControlledCharacter->ActionsFilterComponent->GetCurrentBaseState();
}

void ADustyCameraManager::UpdateBasicLocomotionCamera(float DeltaTime)
{
	if (!CameraOverrider)
	{
		FovController();
		SpringArmResetter(DeltaTime);
		SocketOffsetResetter(DeltaTime);
	}
}

#pragma endregion


#pragma region ClimbMovementRegion

void ADustyCameraManager::InitClimbMovementCamera()
{
	SetModifier(UCM_SmoothMovement::StaticClass(), false);
	SetModifier(UCM_ClimbAutoAdjustment::StaticClass(), true);
}

void ADustyCameraManager::ExitClimbMovementCamera()
{
	SetModifier(UCM_ClimbAutoAdjustment::StaticClass(), false);
	m_LastState = ControlledCharacter->ActionsFilterComponent->GetCurrentBaseState();
}

void ADustyCameraManager::UpdateClimbMovementCamera(float DeltaTime)
{
	FovController();
	SpringArmResetter(DeltaTime);
	SocketOffsetResetter(DeltaTime);
}

#pragma endregion


#pragma region AimRegion

void ADustyCameraManager::InitAimCamera()
{
	SetModifier(UCM_SmoothMovement::StaticClass(), false);
	SetModifier(UCM_Aim::StaticClass(), true);
}

void ADustyCameraManager::ExitAimCamera()
{
	SetModifier(UCM_Aim::StaticClass(), false);
	m_LastState = ControlledCharacter->ActionsFilterComponent->GetCurrentBaseState();
}

void ADustyCameraManager::UpdateAimCamera(float DeltaTime)
{
	FovController();
	SpringArmResetter(DeltaTime);
	SocketOffsetResetter(DeltaTime);

}

#pragma endregion


#pragma region PoleJumpingRegion

void ADustyCameraManager::InitPoleJumpingCamera()
{
	SetModifier(UCM_SmoothMovement::StaticClass(), false);
	SetModifier(UCM_Pole::StaticClass(), true);
}

void ADustyCameraManager::ExitPoleJumpingCamera()
{
	SetModifier(UCM_Pole::StaticClass(), false);
	m_LastState = ControlledCharacter->ActionsFilterComponent->GetCurrentBaseState();
}

void ADustyCameraManager::UpdatePoleJumpingCamera(float DeltaTime)
{
	FovController();
	SpringArmResetter(DeltaTime);
	SocketOffsetResetter(DeltaTime);
}

#pragma endregion


#pragma region CameraUtils


void ADustyCameraManager::SetModifier(TSubclassOf<UCameraModifier> ModifierClass, bool _IsActive)
{
	for (UCameraModifier* Modifier : ModifierList)
	{
		if (Modifier && Modifier->IsA(ModifierClass))
		{
			if (_IsActive)
			{
				Modifier->EnableModifier();
				m_CurrentCameraModifier = ModifierClass;

				// get transition parameters so that we override speed and distances every time
				GetTransitioParameters();
			}
			else
			{
				Modifier->DisableModifier();
			}
			break;
		}
	}
}

void ADustyCameraManager::SetModifier(bool _IsActive)
{
	if (m_CurrentCameraModifier != nullptr)
	{
		SetModifier(m_CurrentCameraModifier, _IsActive);
	}
}

void ADustyCameraManager::EnableModifiers()
{
	for (UCameraModifier* CameraModifier : ModifierList)
	{
		CameraModifier->EnableModifier();
	}
}

void ADustyCameraManager::DisableModifiers()
{
	for (UCameraModifier* CameraModifier : ModifierList)
	{
		CameraModifier->DisableModifier();
	}
}

void ADustyCameraManager::EnableShakeModifier() 
{
	m_IsEnabledShakeModifier = true;
	for (UCameraModifier* CameraModifier : ModifierList)
	{
		if (Cast<UCameraModifier_CameraShake>(CameraModifier))
		{
			CameraModifier->EnableModifier();
		}
	}
}

void ADustyCameraManager::DisableShakeModifier()
{
	m_IsEnabledShakeModifier = false;
	for (UCameraModifier* CameraModifier : ModifierList)
	{
		if (Cast<UCameraModifier_CameraShake>(CameraModifier))
		{
			CameraModifier->DisableModifier();
		}
	}
}


void ADustyCameraManager::SetCurrentOverriderOnWaitValue(bool _newValue)
{
	if (CameraOverrider)
	{
		CameraOverrider->IsOnWait = _newValue;
	}
}


bool ADustyCameraManager::IsCurrentOverriderOnWait() const
{
	if (CameraOverrider)
	{
		return CameraOverrider->IsOnWait;
	}
	return false;
}


void ADustyCameraManager::SetCameraActorBlur(bool value)
{
	FPostProcessSettings& PostProcessSettings = CameraActor->GetCameraComponent()->PostProcessSettings;
	if (value)
	{
		PostProcessSettings.DepthOfFieldFstop = 1.0f;
		PostProcessSettings.DepthOfFieldMinFstop = 1.0f;    
		PostProcessSettings.DepthOfFieldBladeCount = 16.0f;

		PostProcessSettings.DepthOfFieldSensorWidth = 1000.0f;
		PostProcessSettings.DepthOfFieldFocalDistance = 200.0f;
		PostProcessSettings.DepthOfFieldDepthBlurRadius = 60.0f;
	}
	else
	{
		PostProcessSettings.DepthOfFieldFstop = Aperture;
		PostProcessSettings.DepthOfFieldMinFstop = MaxAperture;
		PostProcessSettings.DepthOfFieldBladeCount = DiaphragmBlades;

		PostProcessSettings.DepthOfFieldSensorWidth = SensorWitdh;
		PostProcessSettings.DepthOfFieldFocalDistance = FocalDistance;
		PostProcessSettings.DepthOfFieldDepthBlurRadius = DepthBlurRadius;
	}
}

void ADustyCameraManager::GetTransitioParameters()
{
	m_SpringArmMoveSpeed = BaseSpringArmMoveSpeed;
	m_SocketOffsetMoveSpeed = BaseSocketOffsetMoveSpeed;
	
	m_SpringArmLengthMaxDistance = abs(ControlledCharacter->CameraBoom->TargetArmLength - m_BaseSpringArmLength);
	m_SpringArmSocketOffsetMaxDistance = FVector::Dist(ControlledCharacter->CameraBoom->SocketOffset, m_BaseSpringArmSocketOffset);
	
	float const RequiredArmTime = m_SpringArmLengthMaxDistance / m_SpringArmMoveSpeed;
	float const RequiredOffsetTime = m_SpringArmSocketOffsetMaxDistance / m_SocketOffsetMoveSpeed;

	m_SpringArmIsCorrectlyPlaced = RequiredArmTime == 0.f;
	m_SocketOffsetIsCorrectlyPlaced = RequiredOffsetTime == 0.f;
	
	if (RequiredArmTime > RequiredOffsetTime && RequiredArmTime > 0.f)
	{
		m_SocketOffsetMoveSpeed = m_SpringArmSocketOffsetMaxDistance / RequiredArmTime;
	}
	else if (RequiredOffsetTime > RequiredArmTime && RequiredOffsetTime > 0.f)
	{
		m_SpringArmMoveSpeed = m_SpringArmLengthMaxDistance / RequiredOffsetTime ;
	}
}

void ADustyCameraManager::FovController()
{
	if (ControlledCharacter)
	{
		// get delta rotator
		FRotator const PlayerAimRotation = ControlledCharacter->GetBaseAimRotation();
		FRotator const PlayerWorldRotation = ControlledCharacter->GetCapsuleComponent()->GetComponentRotation();
		FRotator DeltaRotator = (PlayerAimRotation - PlayerWorldRotation);
		DeltaRotator.Normalize();

		if (DeltaRotator.Pitch > 10.f)
		{
			float const TargetFov = DefaultFOV + pow(DeltaRotator.Pitch / ViewPitchMax, 2) * 5;
			SetFOV(TargetFov);
		}
	}
}

// todo: deprecated
void ADustyCameraManager::SpringArmController(float DeltaTime)
{
	if (ControlledCharacter)
	{
		float const MaxSpeed = ControlledCharacter->DustyMovementComponent->MaxWalkSpeed;
		FVector VelocityXY = ControlledCharacter->DustyMovementComponent->Velocity;
		VelocityXY.Z = 0.f;

		// this determines if spring arm is growing or reducing (this won't work if design messes the values)
		bool const TargetModifier = round(VelocityXY.Size() / MaxSpeed) == 1;
		float const TargetSpringArmLength = m_BaseSpringArmLength + (m_MaxSpringArmLength - m_BaseSpringArmLength) * TargetModifier;
		float const Difference = ControlledCharacter->CameraBoom->TargetArmLength - TargetSpringArmLength;

		// if there is no movement then try calling spring arm resetter
		if (!TargetModifier && !m_SpringArmIsCorrectlyPlaced)
		{
			SpringArmResetter(DeltaTime);
			return;
		}
		
		if (Difference != 0.f)
		{
			// the closer to the target the slower the spring arm moves
			if (Difference > 0.f && !TargetModifier)
			{
				ControlledCharacter->CameraBoom->TargetArmLength -= m_SpringArmMoveSpeed * DeltaTime;
			}

			if (Difference < 0.f && TargetModifier)
			{
				ControlledCharacter->CameraBoom->TargetArmLength += m_SpringArmMoveSpeed * DeltaTime;
			}
			
			if (Difference > 0.f && TargetModifier || Difference < 0.f && !TargetModifier)
			{
				ControlledCharacter->CameraBoom->TargetArmLength = TargetSpringArmLength;
			}
		}
	}
}


void ADustyCameraManager::SpringArmResetter(float DeltaTime)
{
	if (ControlledCharacter && !m_SpringArmIsCorrectlyPlaced)
	{
		m_SpringArmIsCorrectlyPlaced = CameraLerp(
			DeltaTime,
			ControlledCharacter->CameraBoom->TargetArmLength,
			m_BaseSpringArmLength,
			m_SpringArmMoveSpeed);
	}
}

void ADustyCameraManager::SocketOffsetResetter(float DeltaTime)
{
	if (ControlledCharacter && !m_SocketOffsetIsCorrectlyPlaced)
	{
		m_SocketOffsetIsCorrectlyPlaced = CameraLerp(
			DeltaTime,
			ControlledCharacter->CameraBoom->SocketOffset,
			m_BaseSpringArmSocketOffset,
			m_SocketOffsetMoveSpeed);
	}
}


FRotator ADustyCameraManager::GetDeltaRotator() const
{
	FRotator const PlayerAimRotation = ControlledCharacter->GetBaseAimRotation();
	FRotator const PlayerWorldRotation = ControlledCharacter->GetCapsuleComponent()->GetComponentRotation();
	FRotator DeltaRotator = (PlayerAimRotation-PlayerWorldRotation);
	DeltaRotator.Normalize();

	// if player controller input is not enough (is to smÖl) then ignore input
	if (abs(ControlledCharacter->MovementInput.X) <= InputOverrider)
	{
		DeltaRotator.Yaw *= 0.f;
	}
	return DeltaRotator;
}

bool ADustyCameraManager::IsOverrided() const
{
	return CameraOverrider != nullptr;
}

bool ADustyCameraManager::CameraLerp(float const& DeltaTime, float& CurrentValue, float const& TargetValue, float const& Speed)
{
	float const TargetDistance = abs(CurrentValue - TargetValue);

	// if distance is Zero
	if (TargetDistance == 0)
	{
		return true;
	}
	
	// get alpha for lerp
	float const Alpha = FMath::Min(DeltaTime * Speed / TargetDistance, 1.0f);
	float const NewLocation = FMath::Lerp(CurrentValue, TargetValue, Alpha);

	// if distance between started and new is bigger then clamp value
	bool const ReachedTarget = abs(CurrentValue - NewLocation)+TOLERANCE >= TargetDistance;

	// set new value to current location
	CurrentValue = (ReachedTarget) ? TargetValue : NewLocation;
	
	return ReachedTarget;
}

bool ADustyCameraManager::CameraLerp(float const& DeltaTime, FVector& CurrentLocation, FVector const& TargetLocation, float const& Speed)
{
	float const TargetDistance = FVector::Dist(CurrentLocation, TargetLocation);

	// if distance is Zero
	if (TargetDistance == 0)
	{
		return true;
	}
	
	// get alpha for lerp
	float const Alpha = FMath::Min(DeltaTime * Speed / TargetDistance, 1.0f);
	FVector const NewLocation = FMath::Lerp(CurrentLocation, TargetLocation, Alpha);

	// if distance between started and new is bigger then clamp value
	bool const ReachedTarget = FVector::Dist(CurrentLocation, NewLocation)+TOLERANCE >= TargetDistance;

	// set new value to current location
	CurrentLocation = (ReachedTarget) ? TargetLocation : NewLocation;
	
	return ReachedTarget;
}

bool ADustyCameraManager::CameraLerp(float const& DeltaTime, FRotator& CurrentRotation, FRotator const& TargetRotation, float const& Speed)
{
	float const TargetDistance = DustyUtils::GetAngularDistance(CurrentRotation, TargetRotation);

	// if distance is Zero
	if (TargetDistance == 0)
	{
		return true;
	}
	
	float const Alpha = FMath::Min(DeltaTime * Speed / TargetDistance, 1.0f);
	FRotator const NewLocation = FMath::Lerp(CurrentRotation, TargetRotation, Alpha);

	// if distance between started and new is bigger then clamp value
	bool const ReachedTarget =  DustyUtils::GetAngularDistance(CurrentRotation, NewLocation)+TOLERANCE >= TargetDistance;

	// set new value to current location
	CurrentRotation = (ReachedTarget) ? TargetRotation : NewLocation;
	
	return ReachedTarget;
}

void ADustyCameraManager::StartCameraInit(const FTransform& _cameraTransform, float _transitionTime)
{
	if (CameraActor == nullptr)
	{

		DustyUtils::PrintMessage("There is no camera actor");
		CameraActor = GetWorld()->SpawnActor<ADustyCameraActor>();
	}
	if (ControlledCharacter == nullptr)
	{
		DustyUtils::PrintMessage("There is no controlled character");
		ControlledCharacter = Cast<ADustyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	}
	if (CameraActor && ControlledCharacter)
	{
		DustyUtils::PrintMessage("Start logic");
		// set default values
		m_StartCameraCurrentLocation = _cameraTransform.GetLocation();
		m_StartCameraCurrentRotation = _cameraTransform.	GetRotation().Rotator();
		m_StartCameraTransitionTime = _transitionTime;

		m_IsOnStartCamera = true;
		m_IsOnStartCameraWait = true;
		
		// set camera at location
		CameraActor->SetActorTransform(_cameraTransform);
		
		// set camera view on camera actor
		GetOwningPlayerController()->SetViewTargetWithBlend(CameraActor, 0.f, VTBlend_Linear);

		// set exit values
		ControlledCharacter->CameraBoom->TargetArmLength = m_BaseSpringArmLength;
		ControlledCharacter->CameraBoom->SocketOffset = m_BaseSpringArmSocketOffset;
		
		FRotator const ExitRotation = FRotator(-20.f, ControlledCharacter->GetActorRotation().Yaw, 0.f);
		GetOwningPlayerController()->SetControlRotation(ExitRotation);
	}
}

void ADustyCameraManager::StartCameraExit()
{
	if (ControlledCharacter && CameraActor)
	{
		m_IsOnStartCamera = true;
		m_IsOnStartCameraWait = false;

		m_StartCameraReachedLocation = false;
		m_StartCameraReachedRotation = false;
		
		m_StartCameraTargetLocation = ControlledCharacter->FollowCamera->GetComponentLocation();
		m_StartCameraTargetRotation = ControlledCharacter->FollowCamera->GetComponentRotation();

		m_StartCameraMoveSpeed = FVector::Dist(m_StartCameraCurrentLocation, m_StartCameraTargetLocation) / m_StartCameraTransitionTime;
		m_StartCameraRotateSpeed = DustyUtils::GetAngularDistance(m_StartCameraCurrentRotation, m_StartCameraTargetRotation) / m_StartCameraTransitionTime;
		
		// disable player's input
		ControlledCharacter->SetInput(false);
	}
}

#pragma endregion
