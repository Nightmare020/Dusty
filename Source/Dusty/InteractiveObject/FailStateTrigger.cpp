#include "FailStateTrigger.h"
#include "../Character/DustyCharacter.h"
#include "Components/BoxComponent.h"

AFailStateTrigger::AFailStateTrigger()
{
	PrimaryActorTick.bCanEverTick = false;

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
}


void AFailStateTrigger::NotifyActorBeginOverlap(AActor* OtherActor)
{
	DustyUtils::PrintMessage("Begin Overlap" + OtherActor->GetName());
	if (ADustyCharacter* Dusty = Cast<ADustyCharacter>(OtherActor))
	{
		DustyUtils::PrintMessage("Start Fade In");
		Dusty->StartFadeIn(Dusty->GetFadeInTime(), &ADustyCharacter::SetBackValidPosition);
	}
}
