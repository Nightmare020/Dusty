// Fill out your copyright notice in the Description page of Project Settings.


#include "LevitatingMesh.h"
#include "Dusty/Utils/DustyUtils.h"

// Sets default values
ALevitatingMesh::ALevitatingMesh()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("Root")));

	m_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ballMesh"));
	m_Mesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ALevitatingMesh::BeginPlay()
{
	Super::BeginPlay();
	m_InitialPosition = GetActorLocation();
}

// Called every frame
void ALevitatingMesh::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	m_FloatingTimer += DeltaTime;
	FVector LittleMovement = DustyUtils::GetRandomMovement(m_InitialPosition, m_FloatingTimer, m_Amplitude, m_Frequency, 4);
	SetActorLocation(LittleMovement);

}

