#include "Interactable.h"
#include "../../Character/DustyCharacter.h"

AInteractable::AInteractable()
{
	PrimaryActorTick.bCanEverTick = false;
}


void AInteractable::Activate(bool _bActivation)
{
	if (_bActivation)
	{
		BP_Activate();
	}
}



void AInteractable::NotifyActorBeginOverlap(AActor* OtherActor)
{
	// Check if the overlapping actor is Dusty
	ADustyCharacter* Character = Cast<ADustyCharacter>(OtherActor);

	if (Character)
	{
		DustyCharacter = Character;
		HasPlayerOverlapped = true;
	}
}

void AInteractable::NotifyActorEndOverlap(AActor* OtherActor)
{
	// Check if the actor that stopped overlapping is Dusty
	if (DustyCharacter && OtherActor == DustyCharacter)
	{
		HasPlayerOverlapped = false;
	}
}