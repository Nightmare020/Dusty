// Fill out your copyright notice in the Description page of Project Settings.

#include "AcademyPressurePlateController.h"
#include "Dusty/InteractiveObject/Interactable/DissolveActor.h"

void AAcademyPressurePlateController::BeginPlay()
{
	Super::BeginPlay();

	for (ADissolveActor* DissolveActor : DissolveEntities)
	{
		if (DissolveActor)
		{
			DissolveActor->Init(false, m_DissolveTime);
		}
	}
}


void AAcademyPressurePlateController::ResolvePuzzle()
{
	Super::ResolvePuzzle();

	for (ADissolveActor* DissolveActor : DissolveEntities)
	{
		if (DissolveActor)
		{
			DissolveActor->Init(false, m_DissolveTime);
		}
	}
}