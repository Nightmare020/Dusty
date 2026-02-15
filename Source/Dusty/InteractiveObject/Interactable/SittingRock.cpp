// Fill out your copyright notice in the Description page of Project Settings.


#include "SittingRock.h"
#include "Components/BoxComponent.h"
#include "Dusty/Character/DustyCharacter.h"
#include "Dusty/Character/CharacterStates/BaseState.h"
#include "Dusty/Character/CharacterStates/ActionsFilter.h"
#include "Components/ArrowComponent.h"

ASittingRock::ASittingRock()
{
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(root);

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(root);
	SitPosition = CreateDefaultSubobject<UArrowComponent>(TEXT("Sitting Position"));
	SitPosition->SetupAttachment(root);
	m_boxTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	m_boxTrigger->SetupAttachment(root);

}

void ASittingRock::SitDusty()
{
	//if (DustyCharacter)
	//{
	//	//DustyCharacter->PlayAnimMontage(SitAnimation);
	//	DustyCharacter->ActionsFilterComponent->SetCurrentState(USittingState::StaticClass());		
	//}
}

void ASittingRock::SitDusty(ADustyCharacter* _dusty)
{
	//if (DustyCharacter == nullptr)
	//{
	//	DustyCharacter = _dusty;
	//}

	//DustyCharacter->SetActorLocation(SitPosition->GetComponentLocation());
	//// FVector(SitPosition->GetComponentLocation().X, SitPosition->GetComponentLocation().Y, DustyCharacter->GetActorLocation().Z));
	//DustyCharacter->SetActorRotation(SitPosition->GetComponentRotation());
	//SitDusty();
}

void ASittingRock::Interact()
{
	//if (CanInteract)
	//{
	//	Super::Interact();
	//	DustyUtils::DustyMoveTo(DustyCharacter, SitPosition->GetComponentLocation(), SitPosition->GetComponentRotation(), &AInteractable::OnFinishedAutoMovement, this);
	//}
}

void ASittingRock::OnFinishedAutoMovement()
{
	//SitDusty();
}