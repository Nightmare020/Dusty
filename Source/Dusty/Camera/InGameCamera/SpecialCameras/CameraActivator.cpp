#include "CameraActivator.h"

#include "Dusty/Camera/InGameCamera/EventCameras/EventCamera.h"
#include "Dusty/Character/DustyCharacter.h"
#include "Dusty/Utils/DustyUtils.h"

ACameraActivator::ACameraActivator()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("Root"));
	
	m_Trigger = CreateDefaultSubobject<UStaticMeshComponent>("Trigger");
	m_Trigger->SetupAttachment(RootComponent);

	DustyUtils::SetAIMeshCOllisions(m_Trigger); // set default colllisions
}

void ACameraActivator::BeginPlay()
{
	Super::BeginPlay();

	SetActorTickEnabled(false); // set default tick
	
	// subscribe mesh to overlap events
	m_Trigger->OnComponentBeginOverlap.AddDynamic(this, &ACameraActivator::TriggerBeginOverlap);
	
}

void ACameraActivator::TriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsActive)
	{
		Player = Cast<ADustyCharacter>(OtherActor);
		if(Player && m_CameraToActivate)
		{
			m_CameraToActivate->IsActive = m_ActivateValue;

			if(m_OneUseOnly)
			{
				IsActive = false;
			}
		}
	}
}
