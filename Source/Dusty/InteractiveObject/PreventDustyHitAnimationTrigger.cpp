// Fill out your copyright notice in the Description page of Project Settings.


#include "PreventDustyHitAnimationTrigger.h"
#include "Dusty/Character/DustyCharacter.h"
#include "Components/BoxComponent.h"

// Sets default values
APreventDustyHitAnimationTrigger::APreventDustyHitAnimationTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	m_Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));

}

void APreventDustyHitAnimationTrigger::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (ADustyCharacter* Dusty = Cast<ADustyCharacter>(OtherActor))
	{
		Dusty->CanPlayHitMontage = false;
	}
}


void APreventDustyHitAnimationTrigger::NotifyActorEndOverlap(AActor* OtherActor)
{
	if (ADustyCharacter* Dusty = Cast<ADustyCharacter>(OtherActor))
	{
		Dusty->CanPlayHitMontage = true;
	}
}


