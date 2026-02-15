// Fill out your copyright notice in the Description page of Project Settings.


#include "Bench.h"
#include "Components/BoxComponent.h"
#include "../../DustyGameInstance.h"
#include "../../Character/DustyCharacter.h"
#include "../../Character/DustyMovementComponent.h"
#include "../../Animations/DustyAnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "../../Cinematics/CinematicsSubsystem.h"
#include "../../Cinematics/BoxCinematicComponent.h"
#include "../../UI/HUD/HUDSubsystem.h"
#include "LevelSequenceActor.h"
#include "../../Character/CharacterStates/ActionsFilter.h"

ABench::ABench()
{
	// Create the mesh component
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Bench Mesh"));
	Mesh->SetupAttachment(RootComponent);

	// Create box collider component
	CollisionMesh = CreateDefaultSubobject<UBoxCinematicComponent>(FName("Bench Box Trigger"));
	CollisionMesh->SetupAttachment(Mesh);

}


void ABench::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Evaluate the Skip Cinematic Possibility
	CollisionMesh->SkipCinematic();
}

void ABench::Interact()
{
	Super::Interact();

	FVector Location = GetActorLocation() + m_DustyPosition;
	FRotator Rotation = GetActorForwardVector().Rotation();

	//First, set the character to the bench center point
	DustyUtils::DustyMoveTo(DustyCharacter, Location, Rotation, &AInteractable::OnFinishedAutoMovement, this);
}

void ABench::SitInBench()
{
	float SitAnimationDuration = DustyCharacter->PlayAnimMontage(SitAnimation);

	// Get HUD Subsystem
	UHUDSubsystem* hudSubsystem = GetGameInstance()->GetSubsystem<UHUDSubsystem>();

	if (SitAnimationDuration > 0)
	{
		// Establish the temporizer to execute the play cinematic function
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ABench::PlayCinematicBench, SitAnimationDuration, false);
	}

	// Enter bench state
	DustyCharacter->ActionsFilterComponent->SetCurrentState(UBenchState::StaticClass());

	// Cast the general game instance to the Dusty game instance class
	UDustyGameInstance* DustyGameInstance = Cast<UDustyGameInstance>(GetGameInstance());

	// If the cast was right, set the game instance location to the checkpoint location and save it
	if (DustyGameInstance)
	{
		DustyGameInstance->CurrentPlayerLocation = GetActorLocation() + m_DustyPosition;
		DustyGameInstance->CameraCheckpoint = m_CheckPointCameraTransform;
		DustyGameInstance->SaveGame();
	}

}

void ABench::PlayCinematicBench()
{
	// Get Cinematics Subsystem
	UCinematicsSubsystem* cinematicSubsystem = GetGameInstance()->GetSubsystem<UCinematicsSubsystem>();

	//DustyUtils::PrintMessage("Play Cinematic");

	cinematicSubsystem->PlayBenchCinematic(CinematicName);
}

void ABench::OnFinishedAutoMovement()
{
	SitInBench();
}
