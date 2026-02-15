// Fill out your copyright notice in the Description page of Project Settings.


#include "ShowFinalQuestionTrigger.h"
#include "Dusty/Character/DustyCharacter.h"
#include "Components/BoxComponent.h"

// Sets default values
AShowFinalQuestionTrigger::AShowFinalQuestionTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetActorTickEnabled(false);

	m_Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
}

void AShowFinalQuestionTrigger::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (ADustyCharacter* Dusty = Cast<ADustyCharacter>(OtherActor))
	{
		Miau = Dusty;
		SetActorTickEnabled(true);
	}
}

void AShowFinalQuestionTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Miau && !Miau->m_DustyInFinalQuestion)
	{
		Miau->ShowFinalQuestion();
		SetActorTickEnabled(false);
	}
}
