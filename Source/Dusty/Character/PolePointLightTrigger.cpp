// Fill out your copyright notice in the Description page of Project Settings.


#include "PolePointLightTrigger.h"
#include "Dusty/Character/DustyCharacter.h"

void APolePointLightTrigger::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if (OtherActor->IsA<ADustyCharacter>())
	{
		ADustyCharacter* dusty = Cast<ADustyCharacter>(OtherActor);
		dusty->TogglePolePointLight();
	}
}