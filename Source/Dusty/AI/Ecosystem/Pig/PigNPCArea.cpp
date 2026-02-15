#include "PigNPCArea.h"

#include "PigNPC.h"
#include "Dusty/Utils/DustyUtils.h"


APigNPCArea::APigNPCArea()
{
	PrimaryActorTick.bCanEverTick = false;

	// base mesh creation
	m_Trigger = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Trigger"));
	m_Trigger->SetupAttachment(RootComponent);

	// set collisions for AI mesh
	DustyUtils::SetAIMeshCOllisions(m_Trigger);
	m_Trigger->CastShadow = false;
}

void APigNPCArea::BeginPlay()
{
	Super::BeginPlay();

	m_Trigger->OnComponentBeginOverlap.AddDynamic(this, &APigNPCArea::OnBeginOverlap);
}

void APigNPCArea::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// if correct actor and target is this Area actor
	APigNPC* TempNPC = Cast<APigNPC>(OtherActor);
	if (TempNPC && TempNPC->CurrentTarget == this)
	{
		if(m_AreaType == AreaType::Dig)
		{
			TempNPC->SetCurrentState(PigNPCMode::Digging);
		}
		else
		{
			TempNPC->SetCurrentState(PigNPCMode::Herding);
		}
	}
}

bool APigNPCArea::IsFree() const
{
	return m_IsFree;
}

void APigNPCArea::SetIsFree(bool _IsFree)
{
	m_IsFree = _IsFree;
}

AreaType APigNPCArea::GetType() const
{
	return m_AreaType;
}

