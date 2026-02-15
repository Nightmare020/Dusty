#include "GlyphTogglePuzzle.h"


void AGlyphTogglePuzzle::Activate()
{
	Super::Activate();
	if (Interactable != nullptr)
	{
		Interactable->SetActorHiddenInGame(false);
		TArray<AActor*> ChildActors;
		Interactable->GetAllChildActors(ChildActors);
		
		for (AActor* Actor : ChildActors)
		{
			Actor->SetActorHiddenInGame(false);
			Actor->SetActorEnableCollision(true);
		}
	}
}


void AGlyphTogglePuzzle::Deactivate()
{
	if (Interactable != nullptr)
	{
		Interactable->SetActorHiddenInGame(true);
		TArray<AActor*> ChildActors;
		Interactable->GetAllChildActors(ChildActors);

		for (AActor* Actor : ChildActors)
		{
			Actor->SetActorHiddenInGame(true);
			Actor->SetActorEnableCollision(false);
		}
	}
}
