// Fill out your copyright notice in the Description page of Project Settings.


#include "Planet.h"
#include "StaticMeshLight.h"
#include "Dusty/Utils/DustyUtils.h"

void APlanet::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(true);
}

APlanet::APlanet()
{
	sateliteOrbit = CreateDefaultSubobject<USceneComponent>("SateliteOrbit");
	sateliteOrbit->SetupAttachment(planetMesh);
	
	sateliteMesh = CreateDefaultSubobject<UStaticMeshComponent>("Satelite");
	sateliteMesh->SetupAttachment(sateliteOrbit);
}

void APlanet::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	sateliteOrbit->SetRelativeRotation(sateliteOrbit->GetRelativeRotation() + FRotator(0,rotationSpeed * DeltaSeconds,0));
}

void APlanet::RotateDisc(bool Direction)
{
	if (Rotating)
	{
		return;
	}
	Rotating = true;
	haveTick = true;
	//SetActorTickEnabled(true);
	if (Direction)
	{
		CurrentGlyph = DustyUtils::ModClamp(CurrentGlyph - 1, 0, 8 - 1);
		m_InitialRotation = GetActorRotation().Yaw;
		m_DesiredRotation = m_InitialRotation +	m_rotationAmount;
	}
	else
	{
		CurrentGlyph = DustyUtils::ModClamp(CurrentGlyph + 1, 0, 8 - 1);
		m_InitialRotation = GetActorRotation().Yaw;
		m_DesiredRotation = m_InitialRotation - m_rotationAmount;
	}
}
