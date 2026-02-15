#include "CameraOverrider.h"

#include "Components/BillboardComponent.h"
#include "Dusty/Character/DustyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Dusty/SaveGame/DustySaveGame.h"
#include "Dusty/DustyGameInstance.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

ACameraOverrider::ACameraOverrider()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACameraOverrider::BeginPlay()
{
	Super::BeginPlay();
	LoadActor();
}

void ACameraOverrider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FRotator ACameraOverrider::GetInputVector()
{
	return FRotator::ZeroRotator; 
}

bool ACameraOverrider::RefChecker()
{
	if (!Player || !CameraManager)
	{
		Player = Cast<ADustyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		CameraManager = Player->CameraManager;
	}

	return Player && CameraManager;
}

void ACameraOverrider::LoadActor()
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
	Ar << IsActive;
}

void ACameraOverrider::OnActorSaved_Implementation()
{
	FActorSaveData ActorData;

	ActorData.ActorName = GetFName();
	ActorData.LevelName = GetLevel()->GetOuter()->GetFName();
	ActorData.Transform = GetActorTransform();
	FMemoryWriter MemWriter(ActorData.ByteData);

	FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
	Ar.ArIsSaveGame = true;
	// Convert binary array back into actor's variables
	Ar << IsActive;


	GetGameInstance<UDustyGameInstance>()->AddActorSaveData(ActorData);
}
