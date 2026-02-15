#include "../Character/DustyLookAtTrigger.h"
#include "Components/BoxComponent.h"
#include "EngineUtils.h"
#include "Dusty/Utils/DustyUtils.h"
#include "../Animations/DustyAnimInstance.h"

ADustyLookAtTrigger::ADustyLookAtTrigger()
{
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("Root"));
	Trigger = CreateDefaultSubobject<UBoxComponent>("Trigger");
	Trigger->SetupAttachment(RootComponent);
}

void ADustyLookAtTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADustyLookAtTrigger::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (!m_DustyCharacter)
	{
		m_DustyCharacter = DustyUtils::GetDustyCharacter();
	}

	if (!m_DustyPlayerAnimInstance)
	{
		m_DustyPlayerAnimInstance = Cast<UDustyAnimInstance>(m_DustyCharacter->GetMesh()->GetAnimInstance());
	}

	if (m_DustyCharacter == Cast<ADustyCharacter>(OtherActor))
	{
		if (ActorToLook)
		{
			m_DustyPlayerAnimInstance->SetDustyLookAt(ActorToLook->GetActorLocation());
		}
		else
		{
			m_DustyPlayerAnimInstance->SetDustyLookAt(TransformToLook.GetLocation() + GetTransform().GetLocation());
		}
		m_TriggerActive = true;
	}
}

void ADustyLookAtTrigger::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	if (!m_DustyCharacter)
	{
		m_DustyCharacter = DustyUtils::GetDustyCharacter();
	}

	if (!m_DustyPlayerAnimInstance)
	{
		m_DustyPlayerAnimInstance = Cast<UDustyAnimInstance>(m_DustyCharacter->GetMesh()->GetAnimInstance());
	}

	if (m_DustyCharacter == Cast<ADustyCharacter>(OtherActor))
	{
		m_TriggerActive = false;
		m_DustyPlayerAnimInstance->RemoveDustyLookAt();
	}

}

void ADustyLookAtTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_TriggerActive)
	{
		if (ActorToLook)
		{
			m_DustyPlayerAnimInstance->SetDustyLookAt(ActorToLook->GetActorLocation());
		}
		else
		{
			m_DustyPlayerAnimInstance->SetDustyLookAt(TransformToLook.GetLocation() +  GetTransform().GetLocation());
		}
	}
}

