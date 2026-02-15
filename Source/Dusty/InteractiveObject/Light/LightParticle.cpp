#include "LightParticle.h"
#include "../../Character/DustyCharacter.h"
#include "../../Utils/DustyConst.h"
#include "Components/PoseableMeshComponent.h"


ALightParticle::ALightParticle()
{
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetMobility(EComponentMobility::Movable);
	//SetRootComponent(mesh);
}


void ALightParticle::BeginPlay()
{
	Super::BeginPlay();
}


void ALightParticle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Character)
	{
		moveToDusty();
	}
	else
	{
		InitialPosition = GetActorLocation();
		FVector CurrentPosition = FMath::Lerp(InitialPosition, FinalPosition, Velocity);
		SetActorLocation(CurrentPosition);
		InitialPosition = CurrentPosition;

		float Distance = (GetActorLocation() - FinalPosition).Size();
		if (Distance <= 100)
		{
			Destroy();
		}
	}
}

void ALightParticle::moveToDusty()
{
	InitialPosition = GetActorLocation();
	FinalPosition = Character->PoleSkeletal->GetBoneLocationByName(BallPoleBoneName, EBoneSpaces::ComponentSpace);
	FVector CurrentPosition = FMath::Lerp(InitialPosition, FinalPosition, Velocity);

	SetActorLocation(CurrentPosition);
	InitialPosition = CurrentPosition;

	float Distance = (GetActorLocation() - FinalPosition).Size();
	if (Distance <= 100)
	{
		Destroy();
	}
}

