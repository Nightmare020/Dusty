// Fill out your copyright notice in the Description page of Project Settings.


#include "Bulb.h"

#include "Components/SphereComponent.h"
#include "Dusty/Character/DustyCharacter.h"

// Sets default values
ABulb::ABulb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	m_mesh->SetupAttachment(RootComponent);

	m_trigger = CreateDefaultSubobject<USphereComponent>("Trigger");
	m_trigger->SetupAttachment(m_mesh);
}

// Called when the game starts or when spawned
void ABulb::BeginPlay()
{
	Super::BeginPlay();
	m_dynamicMaterial = UMaterialInstanceDynamic::Create(m_mesh->GetMaterial(0), this);
	m_mesh->SetMaterial(0, m_dynamicMaterial);
	m_tentacleMovement.SetBlendTime(5.0f);
	m_tentacleMovement.SetCustomCurve(m_tentacleCurve);
	m_tentacleMovement.SetBlendOption(EAlphaBlendOption::Custom);
	m_tentacleMovement.Reset();
}

// Called every frame
void ABulb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//UE_LOG(LogTemp, Warning, TEXT("Value: %f"), m_tentacleCurve->GetFloatValue(m_tentacleProgress));

	float value = 0.0f;
	float value1 = 0.0f;
	float lerpValue = 0.0f;
	if (!m_dustyNear)
	{
		m_tentacleProgress += DeltaTime;
		if(m_tentacleProgress >= m_tentacleBlendTime)
		{
			m_tentacleProgress = m_tentacleBlendTime;
		}
		value = FMath::Lerp(m_dynamicMaterial->K2_GetScalarParameterValue("Sine_Amplitud"), m_tentacleHeight,  m_tentacleProgress/m_tentacleBlendTime);
	}

	else
	{
		m_tentacleProgress -= DeltaTime;
		if(m_tentacleProgress < 0.0f)
		{
			m_tentacleProgress = 0.0f;
		}
		value1 = m_dynamicMaterial->K2_GetScalarParameterValue("Sine_Amplitud");
		lerpValue = 1.0f - m_tentacleProgress/m_tentacleBlendTime;
		value = FMath::Lerp(value1, 0,  lerpValue);
	}
	
	m_dynamicMaterial->SetScalarParameterValue(FName("Sine_Amplitud"), value);

	
}

void ABulb::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if(ADustyCharacter* dusty = Cast<ADustyCharacter>(OtherActor))
	{
		if(!dusty->IsPoleLighted())
		{
			m_dustyNear = true;
		}
	}
}

void ABulb::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);
	if(ADustyCharacter* dusty = Cast<ADustyCharacter>(OtherActor))
	{
		m_dustyNear = false;
	}
}



