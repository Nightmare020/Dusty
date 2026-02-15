// Fill out your copyright notice in the Description page of Project Settings.


#include "UnlockPoleSkillTrigger.h"
#include "Components/BoxComponent.h"
#include "Dusty/Character/DustyCharacter.h"
#include "Dusty/UI/Book/Book3D/BookActorTextures.h"
#include "Dusty/SaveGame/DustySaveGame.h"
#include "Dusty/DustyGameInstance.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

// Sets default values
AUnlockPoleSkillTrigger::AUnlockPoleSkillTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("Root")));

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("UnlockTrigger"));
	Trigger->SetupAttachment(RootComponent);
	Trigger->SetCollisionResponseToChannel(ECC_GameTraceChannel7, ECR_Ignore); //Ignore Audio Collision Channel

}

void AUnlockPoleSkillTrigger::BeginPlay()
{
	Super::BeginPlay();
	LoadActor();
}

void AUnlockPoleSkillTrigger::LoadActor()
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

	if (m_doOnce)
	{
		DustyUtils::GetDustyCharacter()->GetBookActor().LB_UnlockPage(m_pageToUnlock, true);
		SetActorHiddenInGame(true);
		DustyUtils::HideHudWithNoFade(GetWorld(), EHudCanvas::DiaryNotification);
	}
}

void AUnlockPoleSkillTrigger::OnActorSaved_Implementation()
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

void AUnlockPoleSkillTrigger::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (m_doOnce)
	{
		return;
	}


	if (ADustyCharacter* Dusty = Cast<ADustyCharacter>(OtherActor)) //Unlocked skill?
	{
		m_doOnce = true;
		DustyUtils::ShowHud(GetWorld(), EHudCanvas::NewPoleSkill);
		if (ExtendiblePoleSkill)
		{
			Dusty->SetExtendiblePoleSkill();
		}
		if (LightPoleSkill)
		{
			Dusty->SetLightPoleSkill();
		}
		Dusty->GetBookActor().LB_UnlockPage(m_pageToUnlock);

		DustyUtils::HideHudWithNoFade(GetWorld(), EHudCanvas::DiaryNotification);

		SetActorHiddenInGame(true);

	}
}
