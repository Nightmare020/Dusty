#include "ToggleActor.h"

AToggleActor::AToggleActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AToggleActor::Activate()
{
	BP_Activate();
}

//"Activate(bool _bActivation = true)" and we don´t need "Activate()" - Clean code || I also don't understand if we have a deactivate
void AToggleActor::Activate(bool _bActivation)
{
	if (_bActivation)
	{
		Activate();
	}
	else
	{
		Deactivate();
	}
}

void AToggleActor::Activate(bool _bActivation, AActor* _actor)
{
	Activate(_bActivation);
}

void AToggleActor::Deactivate()
{
}
