// Fill out your copyright notice in the Description page of Project Settings.


#include "GrassTrigger.h"

#include "Components/BoxComponent.h"
#include "Dusty/Animations/DustyAnimInstance.h"
#include "Dusty/Character/DustyCharacter.h"

// Sets default values
AGrassTrigger::AGrassTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_trigger = CreateDefaultSubobject<UBoxComponent>("trigger");
	m_trigger->SetupAttachment(RootComponent);
	m_trigger->OnComponentBeginOverlap.AddDynamic(this, &AGrassTrigger::OnBeginOverlap);

}

// Called when the game starts or when spawned
void AGrassTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGrassTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGrassTrigger::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(ADustyCharacter* dusty = Cast<ADustyCharacter>(OtherActor))
	{
		UDustyAnimInstance* dustyAnimInstance = Cast<UDustyAnimInstance>(dusty->GetMesh()->GetAnimInstance());
		dustyAnimInstance->m_lightGrass = activateEffect;
	}
}

