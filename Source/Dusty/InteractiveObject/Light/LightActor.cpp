#include "LightActor.h"
#include "Components/PointLightComponent.h"
#include "Dusty/SaveGame/DustySaveGame.h"
#include "Dusty/DustyGameInstance.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "Dusty/Utils/DustyUtils.h"

ALightActor::ALightActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ALightActor::BeginPlay()
{
	Super::BeginPlay();
	LoadActor();
	SetActorTickEnabled(false);
}

void ALightActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALightActor::Activate(bool Activating)
{
	Super::Activate(Activating);
	Lighted = Activating;

	if (Lighted)
	{
		SetActorTickEnabled(true);
	}
	else
	{
		SetActorTickEnabled(false);
	}
}

void ALightActor::Activate(bool _bActivation, AActor* _actor)
{
	Activate(_bActivation);
}

void ALightActor::LoadActor()
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

	Ar << Lighted;

	//DustyUtils::PrintMessage("Lighted", Lighted);

	Activate(Lighted);
}

void ALightActor::OnActorSaved_Implementation()
{
	FActorSaveData ActorData;

	ActorData.ActorName = GetFName();
	ActorData.LevelName = GetLevel()->GetOuter()->GetFName();
	ActorData.Transform = GetActorTransform();
	//ActorData.Active = Lighted;

	FMemoryWriter MemWriter(ActorData.ByteData);

	FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
	Ar.ArIsSaveGame = true;

	Ar << Lighted;

	GetGameInstance<UDustyGameInstance>()->AddActorSaveData(ActorData);
}

bool ALightActor::GetLightStatus()
{
	return Lighted;
}
