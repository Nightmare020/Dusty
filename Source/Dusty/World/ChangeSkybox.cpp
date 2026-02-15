// Fill out your copyright notice in the Description page of Project Settings.


#include "ChangeSkybox.h"

#include "Components/BoxComponent.h"
#include "Components/SkyAtmosphereComponent.h"
#include "Dusty/Character/DustyCharacter.h"
#include "Engine/ExponentialHeightFog.h"
#include "Components/ExponentialHeightFogComponent.h"

// Sets default values
AChangeSkybox::AChangeSkybox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	trigger = CreateDefaultSubobject<UBoxComponent>("Trigger");
	trigger->SetupAttachment(RootComponent);
	trigger->OnComponentBeginOverlap.AddDynamic(this, &AChangeSkybox::OnBeginOverlap);

}

// Called when the game starts or when spawned
void AChangeSkybox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AChangeSkybox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(modifiyingSkybox && elpasedTime <= lerpDuration)
	{
		elpasedTime += DeltaTime;
		FLinearColor finalColor = FMath::CInterpTo(SkyAtmosphereComponent->GetComponent()->RayleighScattering, skyScattering, DeltaTime, 1);
		SkyAtmosphereComponent->GetComponent()->SetRayleighScattering(FLinearColor(finalColor));
	}

}

void AChangeSkybox::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(Cast<ADustyCharacter>(OtherActor))
	{
		modifiyingSkybox = true;
	}
}

