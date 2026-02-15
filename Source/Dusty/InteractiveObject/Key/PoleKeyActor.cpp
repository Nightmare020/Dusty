// Fill out your copyright notice in the Description page of Project Settings.


#include "PoleKeyActor.h"
#include "Dusty/SaveGame/DustySaveGame.h"
#include "Dusty/DustyGameInstance.h"
#include "Dusty/Utils/DustyConst.h"
#include "Dusty/Character/DustyCharacter.h"
#include "Dusty/Character/CharacterStates/BaseState.h"
#include "Dusty/Character/CharacterStates/ActionsFilter.h"
#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

// Sets default values
APoleKeyActor::APoleKeyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true; 
	
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("InteractionPlatform");
	Mesh->SetupAttachment(Root);

	m_PoleKeyTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("Key Controller Trigger"));
	m_PoleKeyTrigger->SetupAttachment(Mesh);

	ParticlePlatform = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	ParticlePlatform->SetupAttachment(Mesh);


}

// Called when the game starts or when spawned
void APoleKeyActor::BeginPlay()
{
	Super::BeginPlay();

	DustyCharacter = DustyUtils::GetDustyCharacter();
	UpdateMaterials();
}

void APoleKeyActor::LoadActor()
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
	Ar << Unlocked;
}

void APoleKeyActor::OnActorSaved_Implementation()
{
	FActorSaveData ActorData;

	ActorData.ActorName = GetFName();
	ActorData.LevelName = GetLevel()->GetOuter()->GetFName();
	ActorData.Transform = GetActorTransform();
	FMemoryWriter MemWriter(ActorData.ByteData);

	FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
	Ar.ArIsSaveGame = true;
	// Convert binary array back into actor's variables
	Ar << Unlocked;

	GetGameInstance<UDustyGameInstance>()->AddActorSaveData(ActorData);
}

void APoleKeyActor::Interact()
{
	if (!Unlocked)
	{
		return;
	}
	//DustyUtils::HideHud(GetWorld(), EHudCanvas::SingleButtonPrompt);
	Super::Interact();

	FVector Location = GetActorLocation() + m_DustyPosition;
	FRotator Rotation = (GetActorLocation() - Location).Rotation();
	DustyUtils::DustyMoveTo(DustyCharacter, Location, Rotation, &AInteractable::OnFinishedAutoMovement, this);
}

void APoleKeyActor::OnFinishedAutoMovement()
{
	DustyCharacter->ActionsFilterComponent->SetCurrentState(UPoleKeyState::StaticClass());
	if (UPoleKeyState* State = Cast<UPoleKeyState>(DustyCharacter->ActionsFilterComponent->GetCurrentBaseState()))
	{
		State->SetupPoleKeyState(this);
	}
}

void APoleKeyActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	// Check if the overlapping actor is Dusty
	ADustyCharacter* Character = Cast<ADustyCharacter>(OtherActor);

	if (Character)
	{
		DustyCharacter = Character;
		HasPlayerOverlapped = true;

		if (DustyCharacter->ActionsFilterComponent->GetCurrentBaseStateClass() != UAutoMovementState::StaticClass() && Unlocked)
		{
			DustyUtils::ShowHud(GetWorld(), EHudCanvas::SingleButtonPrompt, m_HudButtonId);
		}
		//DustyUtils::PrintMessage(TEXT("Dusty has entered the interactable object overlapping area"), FColor::Green);

	}
}

void APoleKeyActor::NotifyActorEndOverlap(AActor* OtherActor)
{
	if (OtherActor->IsA(ADustyCharacter::StaticClass()))
	{
		DustyUtils::HideHud(GetWorld(), EHudCanvas::SingleButtonPrompt);
	}
}

void APoleKeyActor::OnPoleInserted()
{
	DustyCharacter->CanUnnailPole = true;
}

void APoleKeyActor::OnPoleRemoved()
{
	DustyCharacter->CanUnnailPole = false;
}


void APoleKeyActor::Activate(bool _bActivation)
{
	Super::Activate();
	Unlocked = _bActivation;
	UpdateMaterials();
}

void APoleKeyActor::Deactivate()
{
	Super::Deactivate();
	Unlocked = false;
	UpdateMaterials();
}

// Called every frame
void APoleKeyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APoleKeyActor::Update(float _DeltaTime, FVector2D _Input)
{
}

void APoleKeyActor::UpdateMaterials()
{
	if (!Material)
	{
		Material = Mesh->GetMaterial(0);
		DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);
		Mesh->SetMaterial(0, DynamicMaterial);
		DynamicMaterial->GetVectorParameterValue(DissolveKeyControllerParam, EmissiveIntensity);
	}

	if (Unlocked)
	{
		DynamicMaterial->SetVectorParameterValue(DissolveKeyControllerParam, FLinearColor(EmissiveIntensity.R, EmissiveIntensity.G, EmissiveIntensity.B, EmissiveIntensity.A));
		ParticlePlatform->Activate();
		ParticlePlatform->SetHiddenInGame(false);
	}
	else
	{
		DynamicMaterial->SetVectorParameterValue(DissolveKeyControllerParam, FLinearColor(0, 0, 0, 0));
		
		//Deactivate doesn't work
		//ParticlePlatform->Deactivate();
		ParticlePlatform->SetHiddenInGame(true);
	}
}