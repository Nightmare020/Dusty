#include "PressurePlateDissolveController.h"

#include "Dusty/InteractiveObject/Interactable/DissolveActor.h"

void APressurePlateDissolveController::BeginPlay()
{
	Super::BeginPlay();

	for (ADissolveActor* DissolveActor: DissolveEntities)
	{
		if(DissolveActor)
		{
			DissolveActor->Init(m_IsLighted, m_DissolveTime);
		}	
	}
}
