#include "CameraFocusToAISpawning.h"

#include "Dusty/AI/Maze/Triggers/MazePlayerActivator.h"


ACameraFocusToAISpawning::ACameraFocusToAISpawning()
{
	m_Trigger = CreateDefaultSubobject<UStaticMeshComponent>("TPLocation");
	m_Trigger->SetupAttachment(RootComponent);
}

void ACameraFocusToAISpawning::BeginPlay()
{
	Super::BeginPlay();
	
	m_Trigger->OnComponentBeginOverlap.AddDynamic(this, &ACameraFocusToAISpawning::TriggerBeginOverlap);

}

void ACameraFocusToAISpawning::Reached()
{
	Super::Reached();
	if (m_MazePlayerActivator)
	{
		m_MazePlayerActivator->Activate();
	}
}

void ACameraFocusToAISpawning::TriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(m_ActorClassToCheck)) // if overlapped actor matches the allowed actor
	{
		if (m_MazePlayerActivator)
		{
			Init();
		}
	}
}
