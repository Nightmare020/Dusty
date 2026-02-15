// Fill out your copyright notice in the Description page of Project Settings.


#include "CollectibleUnlocker.h"
#include "NiagaraComponent.h"
#include "Dusty/Character/DustyCharacter.h"
#include "Book3D/BookActorTextures.h"
#include "Components/BoxComponent.h"
#include "Dusty/Audio/AudioSubsytem.h"
#include "Dusty/SaveGame/DustySaveGame.h"
#include "Dusty/DustyGameInstance.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "Components/AudioComponent.h"
#include "Dusty/Character/CharacterStates/ActionsFilter.h"

#if WITH_EDITOR
#include "Components/BillboardComponent.h"
#endif

void ACollectibleUnlocker::LoadActor()
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

	Ar << collected;
	if (collected)
	{
		m_collectibleEffect->SetVisibility(false);
		DustyUtils::GetDustyCharacter()->GetBookActor().LB_UnlockCollectible(m_pageToUnlock, m_collectibleNumber);
	}

	m_AudioComponent->SetActive(!collected);

}

void ACollectibleUnlocker::OnActorSaved_Implementation()
{
	FActorSaveData ActorData;

	ActorData.ActorName = GetFName();
	ActorData.LevelName = GetLevel()->GetOuter()->GetFName();
	ActorData.Transform = GetActorTransform();

	FMemoryWriter MemWriter(ActorData.ByteData);

	FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
	Ar.ArIsSaveGame = true;

	Ar << collected;

	GetGameInstance<UDustyGameInstance>()->AddActorSaveData(ActorData);
}

void ACollectibleUnlocker::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (OtherActor->IsA(ADustyCharacter::StaticClass()) && !collected)
	{
		DustyUtils::ShowHud(GetWorld(), EHudCanvas::SingleButtonPrompt, 4);
	}
}

void ACollectibleUnlocker::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	if (OtherActor->IsA(ADustyCharacter::StaticClass()) && !collected)
	{
		DustyUtils::HideHud(GetWorld(), EHudCanvas::SingleButtonPrompt);
	}
}

void ACollectibleUnlocker::RestartCollectible()
{
	collected = false; 
	m_AudioComponent->SetActive(true);
	m_collectibleEffect->SetVisibility(true);
}

void ACollectibleUnlocker::BeginPlay()
{
	Super::BeginPlay();
	LoadActor();
}

ACollectibleUnlocker::ACollectibleUnlocker():
	Super()
{
	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("Root")));
	m_collectibleEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	m_collectibleEffect->SetupAttachment(RootComponent);

	m_boxTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxTrigger"));
	m_boxTrigger->SetupAttachment(RootComponent);

	m_AudioComponent = CreateDefaultSubobject<UAudioComponent>("Audio Component");
	m_AudioComponent->SetupAttachment(RootComponent);
	

#if WITH_EDITORONLY_DATA
	SpriteComponent = CreateEditorOnlyDefaultSubobject<UBillboardComponent>(TEXT("Sprite"));

	if (!IsRunningCommandlet())
	{
		// Structure to hold one-time initialization
		struct FConstructorStatics
		{
			ConstructorHelpers::FObjectFinderOptional<UTexture2D> SpriteTextureObject;
			FName ID_Effects;
			FText NAME_Effects;
			FConstructorStatics()
				: SpriteTextureObject(TEXT("/Niagara/Icons/S_ParticleSystem"))
				, ID_Effects(TEXT("Effects"))
				, NAME_Effects(NSLOCTEXT("SpriteCategory", "Effects", "Effects"))
			{
			}
		};
		static FConstructorStatics ConstructorStatics;

		if (SpriteComponent)
		{
			SpriteComponent->Sprite = ConstructorStatics.SpriteTextureObject.Get();
			SpriteComponent->SetRelativeScale3D_Direct(FVector(0.5f, 0.5f, 0.5f));
			SpriteComponent->bHiddenInGame = true;
			SpriteComponent->bIsScreenSizeScaled = true;
			SpriteComponent->SpriteInfo.Category = ConstructorStatics.ID_Effects;
			SpriteComponent->SpriteInfo.DisplayName = ConstructorStatics.NAME_Effects;
			SpriteComponent->SetupAttachment(m_collectibleEffect);
			SpriteComponent->bReceivesDecals = false;
		}
	}
#endif
}

void ACollectibleUnlocker::Interact()
{
	Super::Interact();
	if (DustyCharacter && !collected)
	{
		if (DustyCharacter->ActionsFilterComponent->GetCurrentBaseStateClass() == UFallingState::StaticClass())
		{
			return;
		}
		DustyUtils::ShowHud(GetWorld(), EHudCanvas::DiaryNotification, 1);
		UAudioSubsytem* audioSubsystem = GetGameInstance()->GetSubsystem<UAudioSubsytem>();
		audioSubsystem->PlaySound2D(sound);
		m_AudioComponent->SetActive(false);
		m_collectibleEffect->SetVisibility(false);		
		DustyCharacter->GetBookActor().LB_UnlockCollectible(m_pageToUnlock, m_collectibleNumber);
		collected = true;

		DustyUtils::HideHud(GetWorld(), EHudCanvas::SingleButtonPrompt);		
	}
}
