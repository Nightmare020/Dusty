#include "FlowerNPC.h"

#include "Components/CapsuleComponent.h"

AFlowerNPC::AFlowerNPC()
{
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("Root"));
	
	m_SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMesh");
	m_SkeletalMesh->SetupAttachment(RootComponent);
	
	m_Capsule = CreateDefaultSubobject<UCapsuleComponent>("Capsule");
	m_Capsule->SetupAttachment(RootComponent);
}

void AFlowerNPC::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);
}

void AFlowerNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	m_Timer -= DeltaTime;
	if (m_Timer <= 0.f)
	{
		FlowerMode = TempMode;
		SetActorTickEnabled(false);
	}
}

#pragma region Setters / Getters

FlowerNPCMode AFlowerNPC::GetMode() const
{
	return FlowerMode;
}

void AFlowerNPC::SetMode(FlowerNPCMode _newMode, float _time)
{
	TempMode = _newMode;
	m_Timer = _time;

	SetActorTickEnabled(true);
}

#pragma endregion

