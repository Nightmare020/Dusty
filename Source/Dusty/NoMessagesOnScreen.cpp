// Fill out your copyright notice in the Description page of Project Settings.


#include "NoMessagesOnScreen.h"
#include "Kismet\KismetSystemLibrary.h"

// Sets default values
ANoMessagesOnScreen::ANoMessagesOnScreen()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ANoMessagesOnScreen::BeginPlay()
{
	Super::BeginPlay();

#if !WITH_EDITOR
	FString Final = "DisableAllScreenMessages";
	UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), Final);
#endif	
}

// Called every frame
void ANoMessagesOnScreen::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

