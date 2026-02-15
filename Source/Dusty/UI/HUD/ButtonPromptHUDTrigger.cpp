// Fill out your copyright notice in the Description page of Project Settings.


#include "ButtonPromptHUDTrigger.h"
#include "Dusty/Character/DustyCharacter.h"
#include "Dusty/Utils/DustyConst.h"
#include "Dusty/Utils/DustyUtils.h"
#include "Components/BoxComponent.h"

// Sets default values
AButtonPromptHUDTrigger::AButtonPromptHUDTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	m_Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
}


void AButtonPromptHUDTrigger::NotifyActorBeginOverlap(AActor* OtherActor)
{
	ADustyCharacter* Dusty = Cast<ADustyCharacter>(OtherActor);
	if (Dusty == nullptr)
	{
		return;
	}
	if ((m_NeedsExtendSkill && !Dusty->GetExtendiblePoleSkill()) ||
		(m_NeedsLightSkill && !Dusty->GetLightPoleSkill()))
	{
		return;
	}

	if (m_IsDualPrompt)
	{
		DustyUtils::ShowDoublePromptHud(GetWorld(), m_FirstButtonId, m_SecondButtonId);
	}
	else
	{
		DustyUtils::ShowHud(GetWorld(), EHudCanvas::SingleButtonPrompt, m_FirstButtonId);
	}
}

void AButtonPromptHUDTrigger::NotifyActorEndOverlap(AActor* OtherActor)
{
	if (!OtherActor->IsA(ADustyCharacter::StaticClass()))
	{
		return;
	}

	if (m_IsDualPrompt)
	{
		DustyUtils::HideDoublePromptHud(GetWorld());
	}
	else
	{
		DustyUtils::HideHud(GetWorld(), EHudCanvas::SingleButtonPrompt);
	}
}
