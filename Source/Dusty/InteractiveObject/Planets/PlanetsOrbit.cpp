// Fill out your copyright notice in the Description page of Project Settings.


#include "PlanetsOrbit.h"

#include "Dusty/Utils/DustyUtils.h"

APlanetsOrbit::APlanetsOrbit()
{
	SetRootComponent(DiscMesh);

	planetMesh = CreateDefaultSubobject<UStaticMeshComponent>("Planet");
	planetMesh->SetupAttachment(DiscMesh);
}

void APlanetsOrbit::RotatePlanet()
{
}

void APlanetsOrbit::SetOutline(bool Outline)
{
	planetMesh->SetRenderCustomDepth(Outline);
}

void APlanetsOrbit::BeginPlay()
{
	Super::BeginPlay();
}

void APlanetsOrbit::Tick(float DeltaTime)
{
	if(haveTick)
	{
		Super::Tick(DeltaTime);

		//DustyUtils::PrintMessage("Rotating Disc", GetActorRotation(), FColor::Yellow, DeltaTime);

		if (m_Timer < m_TimeToRotate)
		{

			FRotator NewRotation = GetActorRotation();
			m_Timer += DeltaTime;

			NewRotation.Yaw = FMath::Lerp(m_InitialRotation, m_DesiredRotation, m_Timer/m_TimeToRotate);

			SetActorRotation(NewRotation);
		}
		else
		{
			m_Timer = 0;
			FRotator NewRotation = GetActorRotation();
			NewRotation.Yaw = m_DesiredRotation;
			SetActorRotation(NewRotation);

			Rotating = false;
			haveTick = false;
			m_InitialRotation = m_DesiredRotation;
			//SetActorTickEnabled(false);
		}
	}
}

