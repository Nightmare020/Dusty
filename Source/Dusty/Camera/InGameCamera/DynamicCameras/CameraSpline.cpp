#include "CameraSpline.h"

#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/SplineComponent.h"
#include "Dusty/Camera/DustyCameraActor.h"
#include "Dusty/Camera/DustyCameraManager.h"
#include "Dusty/Character/DustyCharacter.h"
#include "Dusty/Character/DustyMovementComponent.h"
#include "Dusty/Utils/DustyUtils.h"
#include "GameFramework/SpringArmComponent.h"


ACameraSpline::ACameraSpline()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("Root"));
	
	Trigger = CreateDefaultSubobject<UStaticMeshComponent>("Trigger");
	Trigger->SetupAttachment(RootComponent);

	m_ExitArrow =  CreateDefaultSubobject<UArrowComponent>("ExitArrow");
	m_ExitArrow->SetupAttachment(RootComponent);

	m_EntryArrow =  CreateDefaultSubobject<UArrowComponent>("EntryArrow");
	m_EntryArrow->SetupAttachment(RootComponent);

	CameraSpline = CreateDefaultSubobject<USplineComponent>("CameraLocation");
	CameraSpline->SetupAttachment(RootComponent);
}


void ACameraSpline::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ACameraSpline::TriggerBeginOverlap);
	Trigger->OnComponentEndOverlap.AddDynamic(this, &ACameraSpline::TriggerEndOverlap);
	
	if (SpeedCurve)
	{
		// set max curve value for speed adjustments
		MaxCurveValue = SpeedCurve->GetFloatValue(1.f);
	}
}


void ACameraSpline::GetDistances(float &RotationDistance, float &MovementDistance, float &FOVDistance) const
{
	MovementDistance = FVector::Dist(m_CurrentLocation, m_TargetLocation);
	RotationDistance = DustyUtils::GetAngularDistance(m_TargetRotation, m_CurrentRotation);
	FOVDistance = m_TargetFOV - CameraManager->CameraActor->GetCameraComponent()->FieldOfView;
}


void ACameraSpline::SetBlendTargets()
{
	if (m_Mode) // if targeting fixed
	{
		m_TargetLocation = CameraSpline->FindLocationClosestToWorldLocation(Player->GetActorLocation(), ESplineCoordinateSpace::World);
		FVector const LookAtDirection = Player->GetActorLocation() - m_TargetLocation;
		m_TargetRotation = FRotationMatrix::MakeFromX(LookAtDirection).Rotator();
	}
	else // if targeting player
	{
		m_TargetLocation = CameraManager->CameraComponent->GetComponentLocation();
		FVector const LookAtDirection = CameraManager->CameraComponent->GetForwardVector();
		m_TargetRotation = FRotationMatrix::MakeFromX(LookAtDirection).Rotator();
	}
}

FRotator ACameraSpline::GetInputVector()
{

	if(!m_MaintainInputDirection)
	{
		return CameraManager->CameraActor->GetActorRotation();
	}
	else
	{
		// maintain input and transform to rotator
		return m_StoredControlRotation;
	}

}


float ACameraSpline::GetNeededCameraMoveSpeed() const 
{
	float const CurrentPlayerSpeed = Player->DustyMovementComponent->Velocity.Size() * PlayerSpeedFactor;
	if(CurrentPlayerSpeed > BaseBlendMoveSpeed)
	{
		return CurrentPlayerSpeed;
	}
	return BaseBlendMoveSpeed;
}



void ACameraSpline::SetBlendParameters()
{
	// get current values for direction vectors
	m_CurrentLocation = CameraManager->CameraActor->GetActorLocation();
	m_CurrentRotation = CameraManager->CameraActor->GetActorRotation();
	m_CurrentFOV = CameraManager->CameraActor->GetCameraComponent()->FieldOfView;
	
	// get distances to each objective to calc needed time
	float RotationDistance, MovementDistance, FOVDistance;
	GetDistances(RotationDistance, MovementDistance, FOVDistance);

	// calc current needed move speed for camera
	float const CurrentMaxMoveSpeed = GetNeededCameraMoveSpeed();
	
	// get needed time for later clamp
	float const MoveTime = MovementDistance / CurrentMaxMoveSpeed;
	float const RotateTime = RotationDistance / BaseBlendRotationSpeed;

	// blend value that will set all speeds
	float BlendTime;
	float SpeedClamp = 1.f;
	if (RotateTime > MoveTime)
	{
		BlendTime = RotateTime;
		// if higher than max allowed speed clamp time
		m_BlendRotationSpeed = RotationDistance / BlendTime;
		if (m_BlendRotationSpeed > BaseBlendRotationSpeed)
		{
			m_BlendRotationSpeed = BaseBlendRotationSpeed;
			BlendTime = RotationDistance / m_BlendRotationSpeed;
		}
		m_BlendMoveSpeed = MovementDistance / BlendTime;

		// if targeting spline get distance to player and calc speed clamp for rotation speed smoothening
		if (SpeedCurve && m_Mode)
		{
			float const MaxRotation = DustyUtils::GetAngularDistance(m_StartingRotation, m_TargetRotation);
			float const NormalizedDistance = FMath::Clamp(RotationDistance / MaxRotation, 0.0f, 1.0f) * MaxCurveValue;
			SpeedClamp = SpeedCurve->GetFloatValue(NormalizedDistance);
		}
	}
	else
	{
		BlendTime = MoveTime;
		// if higher than max allowed speed clamp time
		m_BlendMoveSpeed = MovementDistance/ BlendTime;
		if (m_BlendMoveSpeed > CurrentMaxMoveSpeed)
		{
			m_BlendMoveSpeed = CurrentMaxMoveSpeed;
			BlendTime = MovementDistance / m_BlendMoveSpeed;
		}
		m_BlendRotationSpeed = RotationDistance / BlendTime;

		// if targeting spline get distance to player and calc speed clamp for movement speed smoothening
		if (SpeedCurve && m_Mode)
		{
			float const MaxDistance = FVector::Dist(m_StartingLocation, m_TargetLocation);
			float const NormalizedDistance = FMath::Clamp(MovementDistance / MaxDistance, 0.0f, 1.0f) * MaxCurveValue;
			SpeedClamp = SpeedCurve->GetFloatValue(NormalizedDistance);
		}
	}

	// set all speeds with their current clamp value to adjust to needed speed
	m_BlendMoveSpeed *= SpeedClamp;
	m_BlendRotationSpeed *= SpeedClamp;
	m_BlendFOVSpeed = FOVDistance / BlendTime;
}


void ACameraSpline::Update(float DeltaTime)
{
	// control to know if we need to maintain players input or use the camera input
	if(m_MaintainInputDirection)
	{
		if(Player->MovementInput == FVector2d::ZeroVector)
		{
			m_MaintainInputDirection = false;
		}
	}
	
	if (m_TimerReady)
	{
		//m_OneUseOnlyTimer -= DeltaTime;
		if (!m_InPlace || m_Mode)
		{
			SetBlendTargets(); // get move and rotate targets set in tick
			SetBlendParameters(); // get move, rotate and fov speeds set in tick

			if (!m_Mode)
			{
				// in case we are targeting player we need to add to the speed the player's so it doesn't fall behind
				m_BlendMoveSpeed += Player->DustyMovementComponent->Velocity.Size();
			}

			// get distances for the lerp calc
			float RotationDistance, MovementDistance, FOVDistance;
			GetDistances(RotationDistance, MovementDistance, FOVDistance);

			if (RotationDistance > 0.f)
			{
				m_ReachedRotation = ADustyCameraManager::CameraLerp(DeltaTime, m_CurrentRotation, m_TargetRotation, m_BlendRotationSpeed);
				CameraManager->CameraActor->SetActorRotation(m_CurrentRotation);
			}
		
			if (MovementDistance > 0.f)
			{
				m_ReachedLocation = ADustyCameraManager::CameraLerp(DeltaTime, m_CurrentLocation, m_TargetLocation, m_BlendMoveSpeed);
				CameraManager->CameraActor->SetActorLocation(m_CurrentLocation);
			}

			if (FOVDistance > 0.f)
			{
				float const FAlpha = FMath::Min(DeltaTime * m_BlendFOVSpeed / FOVDistance, 1.0f);
				m_CurrentFOV = FMath::Lerp(m_CurrentFOV, m_TargetFOV, FAlpha);
				CameraManager->CameraActor->SetFov(m_CurrentFOV);
			}
		
			if (m_ReachedLocation && m_ReachedRotation) // if finished
			{
				m_InPlace = true;
				if (!m_Mode) // if targeting player swap cameras
				{
					CameraManager->GetOwningPlayerController()->SetViewTargetWithBlend(
					CameraManager->ControlledCharacter, 0.f, VTBlend_Linear);
					CameraManager->CameraOverrider = nullptr;
					CameraManager->SetModifier(true);
				
					// if (m_OneUseOnlyTimer <= 0.f && m_OneUseOnly) // this means we can disable collisions so it wont be triggered again
					// {
					// 	//Trigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
					// }
				}
			}
		}
	}
}


void ACameraSpline::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// control to know if we need to maintain players input or use the camera input
	if(m_MaintainInputDirection)
	{
		if(Player->MovementInput == FVector2d::ZeroVector)
		{
			m_MaintainInputDirection = false;
		}
	}
	
	if (!DustyActuallyInTrigger && Player)
	{
		FHitResult Hit;
		FVector Start = Player->GetActorLocation();
		FVector End = Start + Player->GetActorForwardVector() * 5;
		if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_GameTraceChannel3, Player->GetIgnoreCharacterCollisionParams()))
		{
			if (Hit.GetActor() == this && CameraManager)
			{
				DustyActuallyInTrigger = true;
				// set mode to target spline
				m_NewState = true;
				m_BlendTimer = BlendDelayTime;
			}
		}
	}
	else
	{
		if (m_BlendTimer <= 0.f)
		{
			if (!CameraManager->CameraOverrider)
			{
				// if targeting spline starting point is the camera component
				m_StartingLocation = CameraManager->CameraComponent->GetComponentLocation();
				m_StartingRotation = CameraManager->CameraComponent->GetComponentRotation();
			}
			else
			{
				// if targeting spline starting point is the camera actor
				m_StartingLocation = CameraManager->CameraActor->GetActorLocation();
				m_StartingRotation = CameraManager->CameraActor->GetActorRotation();
			}
			CameraManager->CameraActor->SetActorLocation(m_StartingLocation);
			CameraManager->CameraActor->SetActorRotation(m_StartingRotation);

			// swap to camera actor for the moving sequence
			CameraManager->GetOwningPlayerController()->SetViewTargetWithBlend(CameraManager->CameraActor, 0.f, VTBlend_Linear);

			SetBlendTargets(); // get initial targets
			CameraManager->CameraOverrider = this;
			m_TimerReady = true;
			
			SetActorTickEnabled(false);
			m_InPlace = false;

			if (m_NewState) // if targeting spline
			{
				// set the starting FOV value
				m_TargetFOV = BlendTargetFOV;
				CameraManager->SetModifier(false);
			}
			else // if targeting player
			{
				// resets camera values for exit procedures
				m_TargetFOV = CameraManager->CameraComponent->FieldOfView;

				Player->CameraBoom->TargetArmLength = CameraManager->m_BaseSpringArmLength;
				m_LastCameraRotation = CameraManager->CameraComponent->GetComponentRotation();
				m_LastCameraLocation = CameraManager->CameraComponent->GetComponentLocation();

				DustyActuallyInTrigger = false;
			}
			m_Mode = m_NewState;

			m_ReachedLocation = false;
			m_ReachedRotation = false;
		}
		else
		{
			m_BlendTimer -= DeltaTime;
		}
	}
	
}


void ACameraSpline::TriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsActive)
	{
		ADustyCharacter* TemporalPlayer = Cast<ADustyCharacter>(OtherActor);

		if (TemporalPlayer)
		{
			//m_OneUseOnlyTimer = m_OneUseOnlyTime;
			Player = TemporalPlayer;
			CameraManager = Player->CameraManager;
			m_TimerReady = false;
			
			// store input just in case
			m_MaintainInputDirection = m_RememberInputDirection;
			m_StoredControlRotation = Player->Controller->GetControlRotation();
			SetActorTickEnabled(true);
		}
	}
}


void ACameraSpline::TriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IsActive)
	{
		// checkers
		if (OtherComp->GetName() == "CapsulePole")
		{
			return;
		}
	
		if (!m_Mode)
		{
			SetActorTickEnabled(false);
			return;
		}

		ADustyCharacter* TemporalPlayer = Cast<ADustyCharacter>(OtherActor);
		if (!TemporalPlayer) // if it is not Dusty
		{
			return;
		}
		
		// get references
		if (RefChecker())
		{
			// set mode to target player
			m_NewState = false;

			FVector const PlayerLocation = Player->GetActorLocation();
			float const DistanceToExitArrow = FVector::Dist(m_ExitArrow->GetComponentLocation(), PlayerLocation);
			float const DistanceToEntryArrow = FVector::Dist(m_EntryArrow->GetComponentLocation(), PlayerLocation);

			// get the exit rotation based on the closest arrow
			const UArrowComponent* SelectedArrow = nullptr;
			bool IsExit = false;
			if (DistanceToEntryArrow < DistanceToExitArrow)
			{
				SelectedArrow = m_EntryArrow;
			}
			else
			{
				SelectedArrow = m_ExitArrow;
				IsExit = true;
			}
		
			FRotator const ExitRotation = FRotator(ExitPitchValue, SelectedArrow->GetComponentRotation().Yaw, 0.f);
			CameraManager->GetOwningPlayerController()->SetControlRotation(ExitRotation);

			// check if it's one use only and it is exit arrow selected
			if (IsExit && m_OneUseOnly)
			{
				Trigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				IsActive = false;
			}
		
			// so that camera keeps moving
			CameraManager->CameraOverrider = this;
			m_TimerReady = false;
			m_BlendTimer = 0.f;

			SetActorTickEnabled(true);
		}
	}
}