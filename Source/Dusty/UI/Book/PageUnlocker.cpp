// Fill out your copyright notice in the Description page of Project Settings.

#include "PageUnlocker.h"
#include "Dusty/Character/DustyCharacter.h"
#include "Book3D/BookActorTextures.h"
#include "Dusty/SaveGame/DustySaveGame.h"
#include "Dusty/DustyGameInstance.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

void APageUnlocker::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if (m_doOnce && OtherActor->IsA<ADustyCharacter>())
	{
		m_doOnce = false;
		ADustyCharacter* dusty = Cast<ADustyCharacter>(OtherActor);
		dusty->GetBookActor().LB_UnlockPage(m_pageToUnlock);
		SetActorHiddenInGame(true);
	}
}

void APageUnlocker::BeginPlay()
{
	Super::BeginPlay();
	LoadActor();
	
}

void APageUnlocker::LoadActor()
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
		ADustyCharacter* dusty = DustyUtils::GetDustyCharacter();
		if (dusty != nullptr)
		{
			dusty->GetBookActor().LB_UnlockPage(m_pageToUnlock, true);
		}
		SetActorHiddenInGame(true);
	}
}

void APageUnlocker::OnActorSaved_Implementation()
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