// Fill out your copyright notice in the Description page of Project Settings.


#include "FixedFailStateTrigger.h"
#include "../Character/DustyCharacter.h"
#include "Components/BoxComponent.h"

// Sets default values
AFixedFailStateTrigger::AFixedFailStateTrigger()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("RootComponent"));

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	Trigger->SetupAttachment(RootComponent);

}
void AFixedFailStateTrigger::BeginPlay()
{
	Super::BeginPlay();
	m_DustyRespawnPosition += GetActorLocation();
}
void AFixedFailStateTrigger::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (ADustyCharacter* Character = Cast<ADustyCharacter>(OtherActor))
	{
		Character->FixedSafePosition = true;
		Character->LastSafePosition = m_DustyRespawnPosition;
	}
}

void AFixedFailStateTrigger::NotifyActorEndOverlap(AActor* OtherActor)
{
	if (ADustyCharacter* Character = Cast<ADustyCharacter>(OtherActor))
	{
		Character->FixedSafePosition = false;
	}

}
