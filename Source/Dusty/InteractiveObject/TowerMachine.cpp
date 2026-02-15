// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerMachine.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Dusty/Audio/AudioSubsytem.h"
#include "Dusty/InteractiveObject/Interactable/DissolveActorController.h"
#include "Dusty/Camera/InGameCamera/EventCameras/CameraFocusToEvent.h"
#include "Dusty/Utils/DustyConst.h"
#include "Dusty/Utils/DustyUtils.h"

ATowerMachine::ATowerMachine()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	USceneComponent* root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(root);

	MachineSphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Machine Sphere"));
	MachineSphere->SetupAttachment(root);
	MachineMonolith = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Machine Monolith"));
	MachineMonolith->SetupAttachment(root);
	MachineStonesA = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Machine Stones A"));
	MachineStonesA->SetupAttachment(root);
	MachineStonesB = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Machine Stones B"));
	MachineStonesB->SetupAttachment(root);
	MachineActiveLights = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Machine Active Lights"));
	MachineActiveLights->SetupAttachment(root);

	m_audioComponent = CreateDefaultSubobject<UAudioComponent>("Audio");
	m_audioComponent->SetupAttachment(root);
}

void ATowerMachine::BeginPlay()
{
	Super::BeginPlay();

	if (MachineStonesA)
	{
		MachineStonesA->SetNiagaraVariableFloat(TEXT("Lifetime"), 0);
	}

	if (MachineActiveLights)
	{
		MachineActiveLights->Deactivate();
		if (ControllerToLight)
		{
			MachineActiveLights->SetWorldLocation(ControllerToLight->GetActorLocation());
			MachineActiveLights->SetNiagaraVariablePosition(TEXT("Origin"), GetActorLocation());
		}
	}

	UMaterialInterface* baseMat = MachineSphere->GetMaterial(0);
	m_dynamicMaterial = UMaterialInstanceDynamic::Create(baseMat, this);
	MachineSphere->SetMaterial(0, m_dynamicMaterial);
	MachineMonolith->SetMaterial(0, m_dynamicMaterial);
}

void ATowerMachine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (state == StonesToMachine)
	{
		m_timer -= DeltaTime;

		if (m_timer <= 0)
		{
			state = LightingUp;
			m_timer = m_lightingUpTimer;
		}
	}
	else if (state == LightingUp)
	{
		m_timer -= DeltaTime;

		float matValue = (1 - ((m_lightingUpTimer - m_timer) / m_lightingUpTimer)) * 0.6;
		m_dynamicMaterial->SetScalarParameterValue(KeyBaseParam, matValue);

		if (m_timer <= 0)
		{
			state = SendingEnergy;
			m_timer = m_sendingEnergyTimer;
			MachineActiveLights->SetVariableFloat(TEXT("Lifetime"), 1);
			MachineActiveLights->SetVariableVec3(FName("Origin"), GetActorLocation());
			MachineActiveLights->Activate();

		}
	}
	else if (state == SendingEnergy)
	{
		m_timer -= DeltaTime;

		if (m_timer <= 0)
		{
			ControllerToLight->Activate(true);
			state = OpeningBridge;
		}
	}
	else if (state == OpeningBridge)
	{
		state = Complete;
	}
	else if (state == Complete)
	{
		SetActorTickEnabled(false);
	}
}

void ATowerMachine::Activate()
{
	Super::Activate();
	if (MachineStonesB)
	{
		MachineStonesA->SetNiagaraVariableFloat(TEXT("Lifetime"), 7);
	}
	Camera->Init();

	m_timer = m_stonesToMachineTimer;
	state = StonesToMachine;
	SetActorTickEnabled(true);

	//Play sound
	if(m_audioComponent)
	{
		GetGameInstance()->GetSubsystem<UAudioSubsytem>()->PlayMetasound(m_audioComponent, FVector::Zero());
	}
	
}

void ATowerMachine::Deactivate()
{
}
