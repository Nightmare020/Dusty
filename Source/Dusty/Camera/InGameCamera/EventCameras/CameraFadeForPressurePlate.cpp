#include "CameraFadeForPressurePlate.h"

#include "Dusty/InteractiveObject/PressurePlate/PressurePlateController.h"


void ACameraFadeForPressurePlate::Reached()
{
	// we need to toggle actor associated 
	if (m_PressurePlateController)
	{
		Super::Reached();
		m_PressurePlateController->CallToResolvePuzzle();
	}
}

