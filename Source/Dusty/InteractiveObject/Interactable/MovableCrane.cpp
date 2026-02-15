// Fill out your copyright notice in the Description page of Project Settings.


#include "MovableCrane.h"
#include "Components/AudioComponent.h"
#include "Dusty/Audio/AudioSubsytem.h"
#include "Dusty/InteractiveObject/Light/Magnetism/MagneticCube.h"

void AMovableCrane::BeginPlay()
{
	Super::BeginPlay();
}

AMovableCrane::AMovableCrane()
{
	childActor = CreateDefaultSubobject<UChildActorComponent>("ChildActor");
	// childActor->bEditableWhenInherited = true;
	// childActor->SetChildActorClass(AMagneticCube::StaticClass());
	// childActor->RegisterComponent();
	// childActor->CreateChildActor();
	childActor->SetupAttachment(RootComponent);
}

void AMovableCrane::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
}

void AMovableCrane::Activate()
{
	m_activated = true;
}

void AMovableCrane::Deactivate()
{
	m_activated = false;
}
