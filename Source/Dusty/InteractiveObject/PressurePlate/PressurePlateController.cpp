#include "PressurePlateController.h"
#include "../ToggleActor.h"
#include "../Interactable/Interactable.h"
#include "Dusty/Camera/InGameCamera/EventCameras/EventCamera.h"
#include "Dusty/SaveGame/DustySaveGame.h"
#include "Dusty/DustyGameInstance.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

#include "Dusty/Character/DustyCharacter.h"
#include "Dusty/InteractiveObject/Interactable/DissolveActor.h"
#include "Dusty/InteractiveObject/Light/LightActor.h"
#include "Dusty/UI/Book/Book3D/BookActorTextures.h"
#include "Dusty/UI/Book/Book2D/PageWidget.h"
#include "Dusty/Utils/DustyUtils.h"

void APressurePlateController::APieceIsSolved()
{
	--m_PiecesToResolve;

	//We prevent to trigger the resolve puzzle if it's already completed in the following sceneario
	//When saving and loading the game, the controller loads as completed and resolves de puzzle
	//But, cubes will spawn on top of the pressure plates, then, try to completed again. And we 
	//don't want to trigger the cameras
	if (m_PiecesToResolve <= 0 && !m_Completed)
	{
		ResolvePuzzleControl();
	}
}

void APressurePlateController::IncreasePiece()
{
	++m_PiecesToResolve;
	
	if (m_PiecesToResolve > 0)
	{
		m_canComplete = false;
	}

	if (m_Completed && !DoOnce)
	{
		UnresolvePuzzle();
	}
}

void APressurePlateController::ResolvePuzzleControl()
{
	if (!m_Completed)
	{
		if (m_EventCamera)
		{
			// delegate control of resolve puzzle in the camera
			m_EventCamera->Init();
		}
		else // since there is no camera call to resolve puzzle normally
		{
			CallToResolvePuzzle();
		}
	}
}

void APressurePlateController::ResolvePuzzle()
{
	if (m_canComplete == true)
	{
		for (int i = 0; i < InteractablesList.Num(); i++)
		{
			if (InteractablesList[i] != nullptr)
			{
				InteractablesList[i]->Activate();
			}
		}

		for (int i = 0; i < RealInteractablesList.Num(); i++)
		{
			if (RealInteractablesList[i] != nullptr)
			{
				RealInteractablesList[i]->Activate();
			}
		}

		m_Completed = true;

		if (PageToUnlock != nullptr || HastoUnlockGlyph == true)
		{
			ABookActorTextures& book = DustyUtils::GetDustyCharacter()->GetBookActor();
			if (HastoUnlockGlyph == true && PageToUnlock != nullptr)
			{
				HasGivenGlyph = true;
				book.LB_UnlockGlyph(GlyphToUnlock, PageToUnlock);
			}
			else if (PageToUnlock != nullptr)
			{
				book.LB_UnlockPage(PageToUnlock);
			}
		}
	}
}

void APressurePlateController::CallToResolvePuzzle()
{
	m_canComplete = true;
	if (m_DelayToActivate > 0.f)
	{
		GetWorld()->GetTimerManager().SetTimer(m_delayToActivateHandle, this, &APressurePlateController::ResolvePuzzle, m_DelayToActivate, false);
	}
	else
	{
		ResolvePuzzle();
	}
}

void APressurePlateController::BeginPlay()
{
	Super::BeginPlay();
	LoadActor();

	for (AToggleActor* ToggleActor: InteractablesList)
	{
		if(ADissolveActor* DissolveActor = Cast<ADissolveActor>(ToggleActor))
		{
			DissolveActor->Init(m_DissolveActorsInitialValue, m_DissolveActorsDissolveTime);
		}
	}
}

void APressurePlateController::LoadActor()
{
	FActorSaveData ActorSaveData = GetGameInstance<UDustyGameInstance>()->GetActorSaveData(GetFName(), GetLevel()->GetOuter()->GetFName());

	if (ActorSaveData.Transform.GetLocation() == FVector::ZeroVector)
	{
		return;
	}
	SetActorTransform(ActorSaveData.Transform);

	FMemoryReader MemReader(ActorSaveData.ByteData);
	FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
	Ar.ArIsSaveGame = true;
	// Convert binary array back into actor's variables
	Ar << m_Completed;
	Ar << DoOnce;
	Ar << HasGivenGlyph;


	if (m_Completed)
	{
		//CallToResolvePuzzle();
		for (int i = 0; i < InteractablesList.Num(); i++)
		{
			if (InteractablesList[i] != nullptr)
			{
				InteractablesList[i]->Activate();
			}
		}

		for (int i = 0; i < RealInteractablesList.Num(); i++)
		{
			if (RealInteractablesList[i] != nullptr)
			{
				RealInteractablesList[i]->Activate();
			}
		}

		
	}
	else
	{
		UnresolvePuzzle();
	}

	if (HasGivenGlyph)
	{
		if (PageToUnlock != nullptr || HastoUnlockGlyph == true)
		{
			ADustyCharacter* dusty = DustyUtils::GetDustyCharacter();
			if (dusty)
			{
				ABookActorTextures& book = dusty->GetBookActor();
				if (HastoUnlockGlyph == true && PageToUnlock != nullptr)
				{
					book.LB_UnlockGlyph(GlyphToUnlock, PageToUnlock, true); //Needed to set true onLoadActor to prevent hud notification
				}
				else if (PageToUnlock != nullptr)
				{
					book.LB_UnlockPage(PageToUnlock);
				}
			}
		}

		DustyUtils::HideHudWithNoFade(GetWorld(), EHudCanvas::DiaryNotification);
	}
}

void APressurePlateController::OnActorSaved_Implementation()
{
	FActorSaveData ActorData;

	ActorData.ActorName = GetFName();
	ActorData.LevelName = GetLevel()->GetOuter()->GetFName();
	ActorData.Transform = GetActorTransform();
	//ActorData.Active = Lighted;

	FMemoryWriter MemWriter(ActorData.ByteData);

	FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
	Ar.ArIsSaveGame = true;
	// Convert binary array back into actor's variables
	Ar << m_Completed;
	Ar << DoOnce;
	Ar << HasGivenGlyph;

	GetGameInstance<UDustyGameInstance>()->AddActorSaveData(ActorData);
}

void APressurePlateController::UnresolvePuzzle()
{
	if (m_Completed)
	{
		for (int i = 0; i < AuxiliarAcademyControllers.Num(); i++)
		{
			if (AuxiliarAcademyControllers[i] != nullptr)
			{
				AuxiliarAcademyControllers[i]->DoOnce = true;
			}
		}

		for (int i = 0; i < InteractablesList.Num(); i++)
		{
			if (InteractablesList[i] != nullptr)
			{
				InteractablesList[i]->Deactivate();
			}
		}

		for (int i = 0; i < RealInteractablesList.Num(); i++)
		{
			if (RealInteractablesList[i] != nullptr)
			{
				RealInteractablesList[i]->Deactivate();
			}
		}

		m_Completed = false;
	}
}

