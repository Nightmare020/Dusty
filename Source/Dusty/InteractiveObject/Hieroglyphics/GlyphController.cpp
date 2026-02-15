#include "GlyphController.h"
#include "../ToggleActor.h"
#include "Dusty/SaveGame/DustySaveGame.h"
#include "Dusty/DustyGameInstance.h"
#include "Dusty/Utils/DustyUtils.h"
#include "Dusty/Character/DustyCharacter.h"
#include "Dusty/UI/Book/Book3D/BookActorTextures.h"
#include "Dusty/UI/Book/Book2D/PageWidget.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"


void AGlyphController::BeginPlay()
{
	Super::BeginPlay();
	LoadActor();
}

void AGlyphController::SetKeyControllerState(AKeyController* _Controller, bool _State)
{
	m_KeyControllers[_Controller] = _State;

	bool CurrentlySolved = true;
	for (TTuple<AKeyController*, bool> KeyController : m_KeyControllers)
	{
		if (!KeyController.Value)
		{
			CurrentlySolved = false;
		}
	}

	if (CurrentlySolved)
	{
		m_Solved = true;
		for (AToggleActor* ActorToActivate : m_ActorsToActivate)
		{
			ActorToActivate->Activate(CurrentlySolved);
		}

		if (PageToUnlock != nullptr)
		{
			ADustyCharacter* dusty = DustyUtils::GetDustyCharacter();
			if (dusty != nullptr)
			{
				dusty->GetBookActor().LB_UnlockPage(PageToUnlock);
			}
		}
	}
}

void AGlyphController::LoadActor()
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
	Ar << m_Solved;
	
	if (m_Solved)
	{
		for (AToggleActor* ActorToActivate : m_ActorsToActivate)
		{
			ActorToActivate->Activate(m_Solved);
		}

		if (PageToUnlock != nullptr)
		{
			ADustyCharacter* dusty = DustyUtils::GetDustyCharacter();
			if (dusty != nullptr)
			{
				dusty->GetBookActor().LB_UnlockPage(PageToUnlock);
			}
		}	
		DustyUtils::HideHudWithNoFade(GetWorld(), EHudCanvas::DiaryNotification);
	}
}

void AGlyphController::OnActorSaved_Implementation()
{
	FActorSaveData ActorData;

	ActorData.ActorName = GetFName();
	ActorData.LevelName = GetLevel()->GetOuter()->GetFName();
	ActorData.Transform = GetActorTransform();
	FMemoryWriter MemWriter(ActorData.ByteData);

	FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
	Ar.ArIsSaveGame = true;
	// Convert binary array back into actor's variables
	Ar << m_Solved;

	GetGameInstance<UDustyGameInstance>()->AddActorSaveData(ActorData);
}
