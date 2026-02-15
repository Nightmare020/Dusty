// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeSpawner.h"
#include "Components/BoxComponent.h"
#include "..\..\InteractiveObject\Light\Magnetism\MagneticCube.h"

// Sets default values
ACubeSpawner::ACubeSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("Root"));


	Trigger = CreateDefaultSubobject<UBoxComponent>("Trigger");
	Trigger->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACubeSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACubeSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACubeSpawner::NotifyActorBeginOverlap(AActor* OtherActor)
{

	AMagneticCube* FirstMagnet = GetWorld()->SpawnActor<AMagneticCube>(MagnetClass, GetActorLocation() +  FirstMagnetismPositon, FRotator::ZeroRotator);
	FirstMagnet->Activate(FirstMagnetismLighted);
	AMagneticCube* SecondMagnet = GetWorld()->SpawnActor<AMagneticCube>(MagnetClass, GetActorLocation() + SecondMagnetismPositon, FRotator::ZeroRotator);
	SecondMagnet->Activate(SecondMagnetismLighted);
	SetActorEnableCollision(false);
}


