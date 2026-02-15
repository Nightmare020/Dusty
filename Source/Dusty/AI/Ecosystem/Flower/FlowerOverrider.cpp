#include "FlowerOverrider.h"

#include "FlowerController.h"
#include "Dusty/Character/DustyCharacter.h"

AFlowerOverrider::AFlowerOverrider()
{
	PrimaryActorTick.bCanEverTick = true;

	
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("Root"));
	
	m_Trigger = CreateDefaultSubobject<UStaticMeshComponent>("BoxComponent");
	m_Trigger->SetupAttachment(RootComponent);

	DustyUtils::SetAIMeshCOllisions(m_Trigger);
}

void AFlowerOverrider::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);
	
	// subscribe mesh to overlap events
	m_Trigger->OnComponentBeginOverlap.AddDynamic(this, &AFlowerOverrider::OnBeginOverlap);
}

void AFlowerOverrider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AFlowerOverrider::GetIsActive() const
{
	return m_IsActive;
}

void AFlowerOverrider::SetIsActive(bool _newIsActive)
{
	m_IsActive = _newIsActive;
}

void AFlowerOverrider::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (m_IsActive)
	{
		m_Player = Cast<ADustyCharacter>(OtherActor);
		if (m_Player)
		{
			// if it's activate then set actor tick to true else false
			bool const TickEnableValue = m_OverrideMode == OverrideMode::Activate;
			
			if (m_FlowerController)
			{
				// initialize the controller
				m_FlowerController->Init(m_Player);
				m_FlowerController->SetActorTickEnabled(TickEnableValue);
			}

			if(m_OtherOverrider) // enable the related trigger
			{
				m_OtherOverrider->SetIsActive(true);
				m_IsActive = false; // disable self
			}
		}
	}
}


