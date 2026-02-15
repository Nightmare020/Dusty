// Fill out your copyright notice in the Description page of Project Settings.


#include "GetDirectionalLine.h"
#include "Engine/DirectionalLight.h"
#include "../Utils/DustyUtils.h"

// Sets default values
AGetDirectionalLine::AGetDirectionalLine()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGetDirectionalLine::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGetDirectionalLine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

#if WITH_EDITOR
	if (DirectionalLightActor)
	{
		FVector Start = GetActorLocation() + GetActorUpVector() * VerticalOffset;
		FVector End = Start + DirectionalLightActor->GetActorRotation().Vector() * LineLength;

		DustyUtils::DustyDrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1, 0, 1);
	}
#endif
}

