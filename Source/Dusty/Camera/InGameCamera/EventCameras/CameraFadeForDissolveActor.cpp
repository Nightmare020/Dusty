#include "CameraFadeForDissolveActor.h"

#include "Dusty/InteractiveObject/Interactable/DissolveActorController.h"

void ACameraFadeForDissolveActor::Reached()
{
	Super::Reached();
	if (m_DissolveController)
	{
		m_DissolveController->Activate();
	}
}
