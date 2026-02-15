// Fill out your copyright notice in the Description page of Project Settings.


#include "GlyphUnlocker.h"
#include "Dusty/Character/DustyCharacter.h"
#include "Book3D/BookActorTextures.h"
#include "Dusty/Utils/DustyUtils.h"
#include "Dusty/SaveGame/DustySaveGame.h"
#include "Dusty/DustyGameInstance.h"
#include "Dusty/Audio/AudioSubsytem.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

void AGlyphUnlocker::BeginPlay()
{
	Super::BeginPlay();
	LoadActor();
}

void AGlyphUnlocker::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	ADustyCharacter* dusty = Cast<ADustyCharacter>(OtherActor);
	if (m_doOnce && dusty)
	{
		m_doOnce = false;
		dusty->GetBookActor().LB_UnlockGlyph(GlyphToUnlock, m_pageToUnlock);
		SetActorHiddenInGame(true);
	}

	//Play sound
	if(m_sound && !m_soundPlayed)
	{
		GetGameInstance()->GetSubsystem<UAudioSubsytem>()->PlaySound2D(m_sound);
		m_soundPlayed = true;
	}
}

void AGlyphUnlocker::LoadActor()
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
	Ar << m_doOnce;
	if (!m_doOnce)
	{
		DustyUtils::GetDustyCharacter()->GetBookActor().LB_UnlockGlyph(GlyphToUnlock, m_pageToUnlock, true);
		SetActorHiddenInGame(true);
	}
}

void AGlyphUnlocker::OnActorSaved_Implementation()
{
	FActorSaveData ActorData;

	ActorData.ActorName = GetFName();
	ActorData.LevelName = GetLevel()->GetOuter()->GetFName();
	ActorData.Transform = GetActorTransform();
	FMemoryWriter MemWriter(ActorData.ByteData);

	FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
	Ar.ArIsSaveGame = true;
	// Convert binary array back into actor's variables
	Ar << m_doOnce;

	GetGameInstance<UDustyGameInstance>()->AddActorSaveData(ActorData);
}
