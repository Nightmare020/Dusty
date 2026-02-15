// Fill out your copyright notice in the Description page of Project Settings.


#include "RayPlanet.h"
#include "Dusty/Utils/DustyUtils.h"
#include "../Interactable/DissolveActorController.h"
#include "PlanetsKeyController.h"
#include "Dusty/SaveGame/DustySaveGame.h"
#include "Dusty/DustyGameInstance.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

ARayPlanet::ARayPlanet()
{
	Root = CreateDefaultSubobject<USceneComponent>("RootComponent");
	SetRootComponent(Root);

	PseudoRoot = CreateDefaultSubobject<USceneComponent>("PseudoRoot");
	PseudoRoot->SetupAttachment(RootComponent);

	PlanetMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	PlanetMesh->SetupAttachment(PseudoRoot);

	m_RayNiagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("RayNiagara"));
}

void ARayPlanet::BeginPlay()
{
	Super::BeginPlay();

	if (m_RayDirection.Num() > 0)
	{
		m_currentTarget = 0;
		m_EmissionDirection = new FEmissionDirections();
		m_EmissionDirection->EmissionDirection = (m_RayDirection[m_currentTarget] - PlanetMesh->GetRelativeLocation()).GetSafeNormal();
		m_EmissionDirection->HittedActor = nullptr;
		m_Emitting = m_InitialEmitting;

		m_NiagaraDirection = m_EmissionDirection->EmissionDirection;
		PlanetMesh->SetRelativeRotation((m_RayDirection[m_currentTarget] - PlanetMesh->GetRelativeLocation()).Rotation());	

	}
	m_CurrentRayDistance = m_FirstRayDistance;
	LoadActor();

	FloatPosition = PlanetMesh->GetComponentLocation();

}

void ARayPlanet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//FVector PlanetPosition = PlanetMesh->GetComponentLocation();
	//PlanetPosition.Z = FloatPosition.Z;
	//FloatPosition = PlanetPosition;

	//m_FloatingTimer += DeltaTime;
	//FVector LittleMovement = DustyUtils::GetRandomMovement(FloatPosition, m_FloatingTimer, m_Amplitude, m_Frequency, 4);
	//PlanetMesh->SetWorldLocation(LittleMovement);

	m_RayNiagara->SetWorldLocation(PlanetMesh->GetComponentLocation());

	if (m_Emitting && m_EmissionDirection)
	{
		FHitResult Hit;
		FVector Start = PlanetMesh->GetComponentLocation();
		FVector End = Start + m_EmissionDirection->EmissionDirection * m_CurrentRayDistance;

		float ActualRayDistance = m_CurrentRayDistance;

		GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_GameTraceChannel1);
		//DustyUtils::DustyDrawDebugLine(GetWorld(), Start, End, Hit.bBlockingHit ? FColor::Green : FColor::Red, false, 0.5);
		if (Hit.bBlockingHit)
		{
			if (Hit.GetActor() && Hit.GetActor()->IsA(AToggleActor::StaticClass()))
			{
				ActualRayDistance = (Start - Hit.ImpactPoint).Size();
			}
			if (m_EmissionDirection->HittedActor && Hit.GetActor() != m_EmissionDirection->HittedActor)
			{
				m_EmissionDirection->HittedActor->Activate(false);
			}
			if (ARayPlanet* PlanetHittedActor = Cast<ARayPlanet>(Hit.GetActor()))
			{
				PlanetHittedActor->Activate(true);

				m_EmissionDirection->HittedActor = PlanetHittedActor;
				if (DustyUtils::GetAngleBetweenVectors(m_EmissionDirection->EmissionDirection, Hit.ImpactNormal) >= 180 - m_AngleThreshold)
				{

				}
			}
			else if (ADissolveActorController* DissolveHittedActor = Cast<ADissolveActorController>(Hit.GetActor()))
			{
				if (DissolveHittedActor != m_EmissionDirection->HittedActor)
				{
					DissolveHittedActor->Activate(true);
					m_EmissionDirection->HittedActor = DissolveHittedActor;
				}
			
			}
		}
		else
		{
			DeactivateOtherActors();
		}

		m_RayNiagara->SetNiagaraVariableVec3(TEXT("Longitud"), m_EmissionDirection->EmissionDirection * ActualRayDistance);


	}

	if(rotatingOrbit)
	{
		if (m_Timer < m_TimeToRotate)
		{
			
			FRotator NewRotation = GetActorRotation();
			m_Timer += DeltaTime;

			float AuxYaw = FMath::Lerp(m_InitialRotation, m_DesiredRotation, m_Timer/m_TimeToRotate);
			m_EmissionDirection->EmissionDirection = m_EmissionDirection->EmissionDirection.RotateAngleAxis(AuxYaw - NewRotation.Yaw, FVector::UpVector);
			NewRotation.Yaw = AuxYaw;

			SetActorRotation(NewRotation);
		}
		else
		{
			m_Timer = 0;
			FRotator NewRotation = GetActorRotation();
			NewRotation.Yaw = m_DesiredRotation;
			SetActorRotation(NewRotation);
			rotatingOrbit = false;
			m_InitialRotation = m_DesiredRotation;
			FloatPosition = PlanetMesh->GetComponentLocation();
			m_EmissionDirection->EmissionDirection = (m_RayDirection[m_currentTarget] - PlanetMesh->GetRelativeLocation()).GetSafeNormal().RotateAngleAxis(GetActorRotation().Yaw, FVector::UpVector);
			//SetActorTickEnabled(false);
		}
	}


	if (m_Rotating)
	{
		m_TimerToRotatePuzzle += DeltaTime;
		FVector NewDirection = FMath::Lerp(m_InitialDirection, m_DesiredDirection, m_TimerToRotatePuzzle / m_TimeToRotatePuzzle);
		PlanetMesh->SetRelativeRotation(NewDirection.Rotation());

		m_EmissionDirection->EmissionDirection = NewDirection;
		if (m_TimerToRotatePuzzle >= m_TimeToRotatePuzzle)
		{
			m_Rotating = false;
			PlanetMesh->SetRelativeRotation(m_DesiredDirection.Rotation());
			if (m_EmissionDirection)
			{
				m_EmissionDirection->EmissionDirection = m_DesiredDirection;
			}
			if (PlanetsKeyController)
			{
				PlanetsKeyController->control = true;
			}
		}

	}


}

void ARayPlanet::Activate(bool Activation)
{
	if (Activation)
	{
		m_Emitting = true;
	}
	else if(!Activation)
	{
		m_Emitting = false;
		m_RayNiagara->SetNiagaraVariableVec3(TEXT("Longitud"), FVector::ZeroVector);
		DeactivateOtherActors();
	}

}

void ARayPlanet::DeactivateOtherActors()
{
	if (m_EmissionDirection && m_EmissionDirection->HittedActor)
	{
		if (ARayPlanet* PlanetHittedActor = Cast<ARayPlanet>(m_EmissionDirection->HittedActor))
		{
			m_EmissionDirection->HittedActor = nullptr;
			PlanetHittedActor->Activate(false);
		}
		else if (ADissolveActorController* DissolveHittedActor = Cast<ADissolveActorController>(m_EmissionDirection->HittedActor))
		{
			m_EmissionDirection->HittedActor = nullptr;
			DissolveHittedActor->Activate(false);

		}
	}
}

void ARayPlanet::SetOutline(bool Outline)
{
	PlanetMesh->SetRenderCustomDepth(Outline);
}

void ARayPlanet::TranslatePlanet(bool _direction)
{
	if (rotatingOrbit)
	{
		return;
	}

	if(_direction)
	{
		rotatingOrbit = true;
		m_InitialRotation = GetActorRotation().Yaw;
		m_DesiredRotation = m_InitialRotation +	m_rotationAmount;
	}
	else
	{
		rotatingOrbit = true;
		m_InitialRotation = GetActorRotation().Yaw;
		m_DesiredRotation = m_InitialRotation -	m_rotationAmount;
	}

}

void ARayPlanet::RotatePlanet(bool _direction)
{
	if(rotatingPlanet)
	{
		return;
	}
	if(_direction)
	{
		rotatingPlanet = true;
		m_InitialRotation = PlanetMesh->GetRelativeRotation().Yaw;
		m_DesiredRotation = m_InitialRotation +	m_rotationAmount;
	}
	else
	{
		rotatingPlanet = true;
		m_InitialRotation = PlanetMesh->GetRelativeRotation().Yaw;
		m_DesiredRotation = m_InitialRotation -	m_rotationAmount;
	}
}

void ARayPlanet::RotatePuzzleCompleted()
{

	if (m_currentTarget < m_RayDirection.Num()-1)
	{
		m_Rotating = true;
		m_InitialDirection = (m_RayDirection[m_currentTarget] - PlanetMesh->GetRelativeLocation()).GetSafeNormal();
		m_DesiredDirection = (m_RayDirection[++m_currentTarget] - PlanetMesh->GetRelativeLocation()).GetSafeNormal();

		m_CurrentRayDistance = m_SecondRayDistance;
	}

}

void ARayPlanet::LoadActor()
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
	Ar << m_currentTarget;
	if (m_EmissionDirection)
	{
		m_EmissionDirection->EmissionDirection = (m_RayDirection[m_currentTarget] - PlanetMesh->GetRelativeLocation()).GetSafeNormal().RotateAngleAxis(GetActorRotation().Yaw, FVector::UpVector);

		m_CurrentRayDistance = m_currentTarget == 0 ? m_FirstRayDistance : m_SecondRayDistance;
	}
}

void ARayPlanet::OnActorSaved_Implementation()
{
	FActorSaveData ActorData;

	ActorData.ActorName = GetFName();
	ActorData.LevelName = GetLevel()->GetOuter()->GetFName();
	ActorData.Transform = GetActorTransform();
	FMemoryWriter MemWriter(ActorData.ByteData);

	FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
	Ar.ArIsSaveGame = true;
	// Convert binary array back into actor's variables
	Ar << m_currentTarget;

	GetGameInstance<UDustyGameInstance>()->AddActorSaveData(ActorData);
}
