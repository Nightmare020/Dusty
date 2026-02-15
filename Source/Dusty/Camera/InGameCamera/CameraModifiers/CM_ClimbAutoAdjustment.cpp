#include "CM_ClimbAutoAdjustment.h"

#include "Dusty/Camera/DustyCameraManager.h"
#include "Dusty/Character/DustyCharacter.h"
#include "GameFramework/SpringArmComponent.h"


bool UCM_ClimbAutoAdjustment::ProcessViewRotation(AActor* ViewTarget, float DeltaTime, FRotator& OutViewRotation, FRotator& OutDeltaRot)
{
	Super::ProcessViewRotation(ViewTarget, DeltaTime, OutViewRotation, OutDeltaRot);
	
	if (ControlledCharacter)
	{
		if (ControlledCharacter->GetLastMovementInputVector().Length() > 0.f)
		{
			if (m_DelayTimer <= 0)
			{
				FRotator const DeltaRotator = DustyCameraManager->GetDeltaRotator();

				// yaw checkers and setters for in tick yaw rotation
				float const YawDifference = DeltaRotator.Yaw;

				FRotator TickRotator = FRotator::ZeroRotator;
			
				if (YawDifference < -1.f && YawDifference > - 179.f)
				{
					TickRotator.Yaw = m_TurnRate + abs(MaxTurnRate * ( YawDifference / 180.f ));
				}
				if(YawDifference > 1.f && YawDifference < 179.f)
				{
					TickRotator.Yaw = (-1) * (m_TurnRate + abs(MaxTurnRate * ( YawDifference / 180.f )));
				}
				
				// pitch checkers and setters for in tick pitch rotation
				float const PitchDifference = DeltaRotator.Pitch;
				//float const AngleDistance = CalculateAngleDistance(PitchDifference, PitchCurrentLocation);
				
				if (PitchDifference < PitchBaseLocation-1 && PitchDifference > - 179.f)
				{
					TickRotator.Pitch = m_TurnRate + abs(MaxTurnRate * ( PitchDifference / 90.f ));
				}
				if(PitchDifference > PitchBaseLocation+1 && PitchDifference < 179.f)
				{
					TickRotator.Pitch = (-1) * ((m_TurnRate) + abs(MaxTurnRate * ( PitchDifference / 90.f )));
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


void UCM_ClimbAutoAdjustment::DisableModifier(bool bImmediate)
{
	Super::DisableModifier(bImmediate);
	m_DelayTimer = DelayBaseTime;
}


void UCM_ClimbAutoAdjustment::EnableModifier()
{
	Super::EnableModifier();

	FRotator const PlayerRotation = DustyCameraManager->ControlledCharacter->GetActorRotation();
	DustyCameraManager->ViewYawMax = PlayerRotation.Yaw + ViewPitchMax;
	DustyCameraManager->ViewYawMin = PlayerRotation.Yaw + ViewPitchMin;

	DustyCameraManager->ControlledCharacter->CameraBoom->SocketOffset = BaseSpringArmSocketOffset;
	DustyCameraManager->ControlledCharacter->CameraBoom->TargetOffset = BaseSpringArmTargetOffset;

}
