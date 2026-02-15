#include "GlyphInteractivePuzzle.h"
#include "Kismet/GameplayStatics.h"
#include "../../Character/DustyCharacter.h"
#include "Components/TextRenderComponent.h"
#include "../../InteractiveObject/Interactable/InteractiveFlower.h"
#include "../Source/Dusty/UI/Book/Book2D/BookWidget.h"

void AGlyphInteractivePuzzle::Interact()
{
	if (!IsHidden())
	{
		SetActorEnableCollision(false);
		TArray<AActor*> ChildActors;
		GetAllChildActors(ChildActors);
		
		for (AActor* Actor : ChildActors)
		{			
			Actor->SetActorHiddenInGame(true);
			Actor->SetActorEnableCollision(false);
		}

		for (size_t i = 0; i < m_Roots.Num(); i++)
		{
			m_Roots[i]->SetActorHiddenInGame(true);
			m_Roots[i]->SetActorEnableCollision(false);
		}

		ADustyCharacter* Dusty = Cast<ADustyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

		if (m_SkyGlyph)
		{
			Dusty->GetBookWidget().ShowGlyphSky();
		}
		else
		{
			Dusty->GetBookWidget().ShowGlyphLight();
		}

		if (InteractiveFlower)
		{
			InteractiveFlower->Activate(false);
		}
		
		SwapText();
	}
}
