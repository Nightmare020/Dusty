// Fill out your copyright notice in the Description page of Project Settings.


#include "Audio2DTrigger.h"

#include "AudioActor.h"
#include "Components/BoxComponent.h"
#include "../Audio/AudioSubsytem.h"
#include "Dusty/Character/DustyCharacter.h"
#include "Dusty/Utils/DustyUtils.h"

// Sets default values
AAudio2DTrigger::AAudio2DTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	trigger = CreateDefaultSubobject<UBoxComponent>("Trigger");
	trigger->SetupAttachment(RootComponent);
	trigger->OnComponentBeginOverlap.AddDynamic(this, &AAudio2DTrigger::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AAudio2DTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAudio2DTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	

}

void AAudio2DTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(!specificAudio)
	{
		ADustyCharacter* dusty = Cast<ADustyCharacter>(OtherActor);
		if(!activated && dusty)
		{
			for(AAudioActor* audio : sounds)
			{
				audio->PlaySound();
				dusty->SetCurrentSound(audio);
			}
			activated = true;
		}
	}

	else
	{
		if(!activated && Cast<ADustyCharacter>(OtherActor))
		{
			for(AAudioActor* audio : sounds)
			{
				audio->PlayTrigger(parameter);
			}
			activated = true;
		}
	}
	
	
}

void AAudio2DTrigger::PlayMetasound_Implementation()
{

}


