// Fill out your copyright notice in the Description page of Project Settings.


#include "TestUIPawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// UI
#include "Blueprint/UserWidget.h"
#include "../Book2D/BookWidget.h"	

// Sets default values
ATestUIPawn::ATestUIPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATestUIPawn::BeginPlay()
{
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(UIMappingContext, 0);
		}
	}

	// Setup Player HUD
	if (m_PlayerHUDWidget)
	{
		PlayerHUDInstance = CreateWidget<UBookWidget>(GetWorld(), m_PlayerHUDWidget);
		PlayerHUDInstance->AddToViewport();
		PlayerHUDInstance->CloseBook();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Player HUD Widget not assigned to UTAD_UI_FPSCharacter"));
#if WITH_EDITOR
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Player HUD Widget not assigned to UTAD_UI_FPSCharacter"));
#endif
	}	
}

// Called every frame
void ATestUIPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ATestUIPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Triggered, this, &ATestUIPawn::Pause);
	}
	else 
	{
#if WITH_EDITOR
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan, "No Input");
#endif
	}
}

void ATestUIPawn::Pause()
{
	PlayerHUDInstance->ToggleBook();
}

