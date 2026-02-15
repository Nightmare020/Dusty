#include "CameraFadeWithTrigger.h"

#include "Dusty/Utils/DustyUtils.h"

ACameraFadeWithTrigger::ACameraFadeWithTrigger()
{
	// base mesh creation
	m_Trigger = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Trigger"));
	m_Trigger->SetupAttachment(RootComponent);

	// set collisions for AI mesh
	DustyUtils::SetAIMeshCOllisions(m_Trigger);
	m_Trigger->CastShadow = false;
}

void ACameraFadeWithTrigger::BeginPlay()
{
	Super::BeginPlay();

	SetActorTickEnabled(false); // set default tick
	
	// subscribe mesh to overlap events
	m_Trigger->OnComponentBeginOverlap.AddDynamic(this, &ACameraFadeWithTrigger::OnBeginOverlap);
}

void ACameraFadeWithTrigger::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(m_ActorClassToCheck))
	{
		Init();
	}
}
