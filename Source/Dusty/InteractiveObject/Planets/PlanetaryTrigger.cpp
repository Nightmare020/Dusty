// Fill out your copyright notice in the Description page of Project Settings.


#include "PlanetaryTrigger.h"
#include "Components/BoxComponent.h"
#include "Dusty/InteractiveObject/Key/PlanetsKeyController.h"
#include "Dusty/Character/DustyCharacter.h"

// Sets default values
APlanetaryTrigger::APlanetaryTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_trigger = CreateDefaultSubobject<UBoxComponent>("Trigger");
	m_trigger->SetupAttachment(RootComponent);

	m_trigger->OnComponentBeginOverlap.AddDynamic(this, &APlanetaryTrigger::RotatePlanets);
}

// Called when the game starts or when spawned
void APlanetaryTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlanetaryTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlanetaryTrigger::RotatePlanets(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(Cast<ADustyCharacter>(OtherActor) && m_active)
	{
		planetController->NeedsToRotatePlanets = true;
		m_active = false;
	}
}

