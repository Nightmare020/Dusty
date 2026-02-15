// Fill out your copyright notice in the Description page of Project Settings.


#include "NiagaraFXTrigger.h"

#include "NiagaraComponent.h"
#include "Components/BoxComponent.h"
#include "Dusty/Character/DustyCharacter.h"

// Sets default values
ANiagaraFXTrigger::ANiagaraFXTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_trigger = CreateDefaultSubobject<UBoxComponent>("Trigger");
	SetRootComponent(m_trigger);
	m_trigger->OnComponentBeginOverlap.AddDynamic(this, &ANiagaraFXTrigger::OnOverlapBegin);

	m_effect = CreateDefaultSubobject<UNiagaraComponent>("FX");
	m_effect->SetupAttachment(m_trigger);
}

// Called when the game starts or when spawned
void ANiagaraFXTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANiagaraFXTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANiagaraFXTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(ADustyCharacter* dusty = Cast<ADustyCharacter>(OtherActor))
	{
		if(!m_activated)
		{
			m_activated = true;
			m_effect->Activate();
		}
	}
	
}

