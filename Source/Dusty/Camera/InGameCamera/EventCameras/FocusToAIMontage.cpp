#include "FocusToAIMontage.h"

#include "Dusty/AI/Maze/MazeNPC.h"


AFocusToAIMontage::AFocusToAIMontage()
{
	m_Trigger = CreateDefaultSubobject<UStaticMeshComponent>("Trigger");
	m_Trigger->SetupAttachment(RootComponent);
}

void AFocusToAIMontage::BeginPlay()
{
	Super::BeginPlay();
	
	m_Trigger->OnComponentBeginOverlap.AddDynamic(this, &AFocusToAIMontage::TriggerBeginOverlap);
	
}

void AFocusToAIMontage::Reached()
{
	Super::Reached();

	if(m_MazeNPC)
	{
		if (const USkeletalMeshComponent* SkeletalMesh = m_MazeNPC->GetMesh())
		{
			if (UAnimInstance* AnimInstance = SkeletalMesh->GetAnimInstance())
			{
				AnimInstance->Montage_Play(m_Montage);
			}
		}
	}
}

void AFocusToAIMontage::TriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(m_ActorClassToCheck)) // if overlapped actor matches the allowed actor
	{
		Init();
	}
}