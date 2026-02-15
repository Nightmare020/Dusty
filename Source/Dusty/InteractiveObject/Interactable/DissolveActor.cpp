#include "DissolveActor.h"

#include "Dusty/Utils/DustyConst.h"
#include "Dusty/SaveGame/DustySaveGame.h"
#include "Dusty/DustyGameInstance.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "Dusty/Utils/DustyUtils.h"

ADissolveActor::ADissolveActor()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("Root"));
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(RootComponent);

	// this entity can only be activated through its controller
	CanBeActivatedByPlayer = false;
}

void ADissolveActor::Activate()
{
	// just a toggle nothing to see here
	Super::Activate();
	Activate(!m_IsActive);
}

void ADissolveActor::Deactivate()
{
	Activate(false);
}

void ADissolveActor::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(true);

	LoadActor();
	m_InitialPosition = GetActorLocation();
}

void ADissolveActor::Activate(bool _bActivation)
{
	SetActorHiddenInGame(false);
	if (m_IsActive && !_bActivation) // deactivate only if active
	{
		// set direction to reduce dissolve value
		m_IsActive = false;
		m_Direction = 1;
			
		// set inverse time used to be time left 
		m_Timer = DissolveTime - m_Timer;
		
		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		m_startDissolve = true;
	}
	else if(_bActivation) // activate door
	{
		// set direction to grow dissolve value
		m_IsActive = true;
		m_Direction = -1;
		
		// set inverse time used to be time left 
		m_Timer = DissolveTime - m_Timer;
		m_startDissolve = true;
	}
}

void ADissolveActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_FloatingMovement)
	{
		m_FloatingTimer += DeltaTime;
		FVector LittleMovement = DustyUtils::GetRandomMovement(m_InitialPosition, m_FloatingTimer, m_Amplitude, m_Frequency, 4);
		SetActorLocation(LittleMovement);
	}

	if (!m_startDissolve)
	{
		return;
	}

	m_Timer += DeltaTime;
	
	// calc new extra value using current time and direction
	float const DissolveStep = m_Direction * (MaxDissolveValue / DissolveTime) * DeltaTime;
	m_CurrentDissolveValue += DissolveStep;
	// clamp just in case
	m_CurrentDissolveValue = FMath::Clamp(m_CurrentDissolveValue, 0.0f, MaxDissolveValue);
	
	for (UMaterialInstanceDynamic* DynamicMaterial: DynamicMaterials)
	{
		if (DynamicMaterial)
		{
			DynamicMaterial->SetScalarParameterValue(DissolveDoorParam, m_CurrentDissolveValue);
		}
	}
	
	if (m_Timer >= DissolveTime) // when timer has reached finish
	{
		//SetActorTickEnabled(false);

		if (m_IsActive)
		{
			Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // if active activate physics
		}
		// reset timer
		m_Timer = DissolveTime;
		m_startDissolve = false;

		if (m_IsActive)
		{
			SetActorHiddenInGame(false);
		}
		else
		{
			SetActorHiddenInGame(true);
		}

	}

}

void ADissolveActor::Init(bool _IsActive, float _Duration)
{
	// we use init instead of begin play because starting situation of actor is dependant of it's controller
	m_IsActive = _IsActive;
	DissolveTime = _Duration;
	m_startDissolve = false;

	
	// create dynamic material array 
	const int NumMaterials = Mesh->GetNumMaterials();
	for (int i=0; i<NumMaterials; ++i)
	{
		DynamicMaterials.Add(UMaterialInstanceDynamic::Create(Mesh->GetMaterial(i), this));
	}

	if (NumMaterials == DynamicMaterials.Num())
	{
		m_Timer = DissolveTime; // timer to max value so it then is m_timer - DissolveTime = 0
		if(m_IsActive)
		{
			m_CurrentDissolveValue = 0.f; // default lowest value
			Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		}
		else
		{
			m_CurrentDissolveValue = MaxDissolveValue; // defined highest value
			Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			SetActorHiddenInGame(true);
		}

		for (int i=0; i<NumMaterials; ++i)
		{
			if (UMaterialInstanceDynamic* DynamicMaterial = DynamicMaterials[i])
			{
				Mesh->SetMaterial(i, DynamicMaterial);
				
				// set value on dynamic material
				DynamicMaterial->SetScalarParameterValue(DissolveDoorParam, m_CurrentDissolveValue);
			}
		}
	}
}

void ADissolveActor::LoadActor()
{
	FActorSaveData ActorSaveData = GetGameInstance<UDustyGameInstance>()->GetActorSaveData(GetFName(), GetLevel()->GetOuter()->GetFName());

	if (ActorSaveData.Transform.GetLocation() == FVector::ZeroVector)
	{
		return;
	}
	SetActorTransform(ActorSaveData.Transform);

	FMemoryReader MemReader(ActorSaveData.ByteData);
	FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
	Ar.ArIsSaveGame = true;
	// Convert binary array back into actor's variables
	Ar << m_IsActive;

	Activate(m_IsActive);
}

void ADissolveActor::OnActorSaved_Implementation()
{
	FActorSaveData ActorData;

	ActorData.ActorName = GetFName();
	ActorData.LevelName = GetLevel()->GetOuter()->GetFName();
	ActorData.Transform = GetActorTransform();
	FMemoryWriter MemWriter(ActorData.ByteData);

	FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
	Ar.ArIsSaveGame = true;
	// Convert binary array back into actor's variables
	Ar << m_IsActive;

	GetGameInstance<UDustyGameInstance>()->AddActorSaveData(ActorData);
}

void ADissolveActor::StartDissolve()
{
	if (m_startDissolve == false)
	{
		Activate(false);
	}

	m_startDissolve = true;
}