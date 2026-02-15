#include "CM_SmoothMovement.h"


#include "Dusty/Camera/DustyCameraManager.h"
#include "Dusty/Character/DustyCharacter.h"


float CalculateAngleDistance(float const Angle1, float const Angle2)
{
	// Calculate the absolute difference between the angles
	float AbsoluteDifference = std::abs(Angle1 - Angle2);

	// If the absolute difference is greater than 90, subtract it from 180
	if (AbsoluteDifference > 90.0f)
	{
		AbsoluteDifference = 180.0f - AbsoluteDifference;
	}
	return AbsoluteDifference;
}



bool UCM_SmoothMovement::ProcessViewRotation(AActor* ViewTarget, float DeltaTime, FRotator& OutViewRotation, FRotator& OutDeltaRot)
{
	if (ControlledCharacter && DustyCameraManager && !DustyCameraManager->IsOverrided())
	{
		if (ControlledCharacter->GetLastMovementInputVector().Length() > 0.1f)
		{
			if (m_DelayTimer <= 0)
			{
				AngleController(); // gets the respective angle from surface for pitch adjustment
				FRotator const DeltaRotator = DustyCameraManager->GetDeltaRotator();
				
				// yaw checkers and setters for in tick yaw rotation
				float const YawDifference = DeltaRotator.Yaw;

				FRotator TickRotator = FRotator::ZeroRotator;
		
				if (YawDifference < -10.f && YawDifference > - 169.f)
				{
					TickRotator.Yaw = MaxYawTurnRate + abs(MaxYawTurnRate * ( YawDifference / 180.f ));
				}
				if (YawDifference > 10.f && YawDifference < 169.f)
				{
					TickRotator.Yaw = (-1) * ( MaxYawTurnRate + abs(MaxYawTurnRate * ( YawDifference / 180.f )));
				}
			
				// pitch checkers and setters for in tick pitch rotation
				float const PitchDifference = DeltaRotator.Pitch;
				float const AngleDistance = CalculateAngleDistance(PitchDifference, m_PitchCurrentLocation);
			
				if (PitchDifference < m_PitchCurrentLocation-1 && PitchDifference > - 179.f)
				{
					TickRotator.Pitch = m_PitchTurnRate + abs(MaxPitchTurnRate * ( AngleDistance / 90.f ));
				}
				if (PitchDifference > m_PitchCurrentLocation+1 && PitchDifference < 179.f)
				{
					TickRotator.Pitch = (-1) * (( m_PitchTurnRate) + abs(MaxPitchTurnRate * ( AngleDistance / 90.f )));
				}

				// apply rotation
				OutDeltaRot = TickRotator * DeltaTime;
			}
			else
			{
				m_DelayTimer -= DeltaTime;
			}
		}
	}
	
	return false;
}


void UCM_SmoothMovement::DisableModifier(bool bImmediate)
{
	Super::DisableModifier(bImmediate);
	m_DelayTimer = DelayBaseTime;
}

void UCM_SmoothMovement::AngleController()
{
	if (ControlledCharacter)
	{
		float const CurrentHeight = ControlledCharacter->GetActorLocation().Z - m_LastHeight;
		m_LastHeight = ControlledCharacter->GetActorLocation().Z;
		if (CurrentHeight != 0.f)
		{
			FHitResult OutHit;
			FVector const TraceStart = ControlledCharacter->GetActorLocation();
			FVector const TraceEnd = ControlledCharacter->GetActorLocation() - FVector(0.f, 0.f, 500.f);

			// ignore dusty character
			FCollisionQueryParams QueryParams;
			QueryParams.AddIgnoredActor(ControlledCharacter);
			
			GetWorld()->LineTraceSingleByChannel(OutHit, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, QueryParams);
			
			if (OutHit.bBlockingHit)
			{
				float FloorAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(OutHit.ImpactNormal, FVector::UpVector)))/2;
				
				if (CurrentHeight < 0.f )
				{
					FloorAngle *= -1.f;
				}
				SetPitchBaseLocation(FloorAngle);
			}
		}
	}
}