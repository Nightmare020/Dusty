// Fill out your copyright notice in the Description page of Project Settings.


#include "TooltipTrigger.h"
#include "Dusty/Character/DustyCharacter.h"
#include "Dusty/Utils/DustyConst.h"
#include "Dusty/Utils/DustyUtils.h"
#include "Components/BoxComponent.h"

// Sets default values
ATooltipTrigger::ATooltipTrigger()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	m_Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));

}



void ATooltipTrigger::NotifyActorBeginOverlap(AActor* OtherActor)
{
	ADustyCharacter* Dusty = Cast<ADustyCharacter>(OtherActor);
	if (Dusty == nullptr)
	{
		return;
	}
	Iterations = (Iterations + 1) % TimesBeforeShow;

	DustyUtils::PrintMessage("Entered", Iterations);

	if(Iterations == 0)
	{
		DustyUtils::ShowHud(GetWorld(), EHudCanvas::Tooltip);
	}
}

