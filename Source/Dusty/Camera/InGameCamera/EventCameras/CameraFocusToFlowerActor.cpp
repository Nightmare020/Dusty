#include "CameraFocusToFlowerActor.h"

#include "Dusty/InteractiveObject/Interactable/InteractiveFlower.h"


void ACameraFocusToFlowerActor::Reached()
{
	// if there is a toggle actor associated call for it's activate with the stored value
	if (m_FlowerActor != nullptr)
	{
		m_FlowerActor->Activate(m_AssociatedToggleActorActivateValue);
	}
}

void ACameraFocusToFlowerActor::Exited()
{
	Super::Exited();
	if (m_FlowerActor)
	{
		// erase only the link from the flower in case later on we want to reactivate it
		m_FlowerActor->FocusCameraToFLower = nullptr;
	}
}

void ACameraFocusToFlowerActor::SetActivateValue(bool _associatedToggleActorActivateValue)
{
	m_AssociatedToggleActorActivateValue = _associatedToggleActorActivateValue;
}
