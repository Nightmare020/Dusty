#include "CameraFocusWithTrigger.h"

ACameraFocusWithTrigger::ACameraFocusWithTrigger()
{
	m_Trigger = CreateDefaultSubobject<UStaticMeshComponent>("Trigger");
	m_Trigger->SetupAttachment(RootComponent);
}

void ACameraFocusWithTrigger::BeginPlay()
{
	Super::BeginPlay();
	
	m_Trigger->OnComponentBeginOverlap.AddDynamic(this, &ACameraFocusWithTrigger::TriggerBeginOverlap);
	
}

void ACameraFocusWithTrigger::TriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(m_ActorClassToCheck)) // if overlapped actor matches the allowed actor
	{
		Init();
	}
}
