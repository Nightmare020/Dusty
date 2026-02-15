#include "MazeActorTrigger.h"

#include "Dusty/Utils/DustyUtils.h"
#include "Dusty/Character/DustyCharacter.h"


AMazeActorTrigger::AMazeActorTrigger()
{
	// base mesh creation
	m_Trigger = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Trigger"));
	m_Trigger->SetupAttachment(RootComponent);

	// set collisions for AI mesh
	DustyUtils::SetAIMeshCOllisions(m_Trigger);
	m_Trigger->CastShadow = false;

}

void AMazeActorTrigger::BeginPlay()
{
	Super::BeginPlay();

	SetActorTickEnabled(false); // set default tick
	
	// subscribe mesh to overlap events
	m_Trigger->OnComponentBeginOverlap.AddDynamic(this, &AMazeActorTrigger::OnBeginOverlap);
	m_Trigger->OnComponentEndOverlap.AddDynamic(this, &AMazeActorTrigger::OnEndOverlap);
}

void AMazeActorTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (m_CheckDusty && DustyUtils::DustyActuallyInsideTrigger(Player, this))
	{
		m_CheckDusty = false;
		DoBeginOverlap(Player);
		SetActorTickEnabled(false);
	}
}

void AMazeActorTrigger::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsActive)
	{
		IsValidOverlap = OtherActor->IsA(m_ActorClassToCheck);

		if (m_ActorClassToCheck == ADustyCharacter::StaticClass())
		{
			ADustyCharacter* AuxPlayer = Cast<ADustyCharacter>(OtherActor);
			if (AuxPlayer)
			{
				Player = AuxPlayer;
				SetActorTickEnabled(true);
				m_CheckDusty = true;
			}
		}
		else if (IsValidOverlap)
		{
			//todo: Pole triggering control
			DoBeginOverlap(OtherActor);
		}
	}
}

void AMazeActorTrigger::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IsActive)
	{
		IsValidOverlap = OtherActor->IsA(m_ActorClassToCheck);

		if(IsValidOverlap)
		{
			//todo: Pole triggering control
			DoEndOverlap(OtherActor);
		}
	}
}