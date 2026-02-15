#include "DustyCameraModifier.h"

#include "Dusty/Camera/DustyCameraManager.h"


void UDustyCameraModifier::Init(ADustyCameraManager* _DustyCameraManager, ADustyCharacter* _ControlledCharacter)
{
	DustyCameraManager = _DustyCameraManager;
	ControlledCharacter = _ControlledCharacter;
}

void UDustyCameraModifier::EnableModifier()
{
	Super::EnableModifier();
	if (ControlledCharacter && DustyCameraManager)
	{
		DustyCameraManager->ViewPitchMax = ViewPitchMax;
		DustyCameraManager->ViewPitchMin = ViewPitchMin;
		DustyCameraManager->ViewYawMax = ViewYawMax;
		DustyCameraManager->ViewYawMin = ViewYawMin;
		
		DustyCameraManager->m_BaseSpringArmLength = BaseSpringArmLength;
		DustyCameraManager->m_BaseSpringArmSocketOffset = BaseSpringArmSocketOffset;
		DustyCameraManager->m_BaseSpringArmTargetOffset = BaseSpringArmTargetOffset;
		
		// just in case it needs adjustment
		DustyCameraManager->m_SpringArmIsCorrectlyPlaced = false;
		DustyCameraManager->m_SocketOffsetIsCorrectlyPlaced = false;

	}
}