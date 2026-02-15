#include "PlanetsKeyController.h"
#include "Disc.h"
#include "RayPlanet.h"
#include "Dusty/Character/CharacterStates/BaseState.h"
#include "../../Character/CharacterStates/ActionsFilter.h"
#include "Dusty/SaveGame/DustySaveGame.h"
#include "Dusty/DustyGameInstance.h"
#include "Dusty/Camera/DustyCameraManager.h"
#include "Dusty/Camera/InGameCamera/SpecialCameras/PlanetaryCamera.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/ExponentialHeightFog.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Dusty/Audio/AudioSubsytem.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

APlanetsKeyController::APlanetsKeyController()
{
	platform = CreateDefaultSubobject<UStaticMeshComponent>("Platform");
	platform->SetupAttachment(Mesh);

	m_platformSound = CreateDefaultSubobject<UAudioComponent>("Audio");
	m_platformSound->SetupAttachment(Mesh);
}

void APlanetsKeyController::BeginPlay()
{
	Super::BeginPlay();
	
	const FVector InitialPlatformPosition = GetActorLocation();
	// add actor location since its local location the final pos
	FinalPlatformPosition += GetActorLocation();
	// get max distance for normalization in curve
	MaxDistance = FVector::Dist(InitialPlatformPosition, FinalPlatformPosition);
	
	LoadActor();

	MAX_DISCS = RayPlanets.Num();

	for (ARayPlanet* planet : RayPlanets)
	{
		if (planet)
		{
			planet->PlanetsKeyController = this;
		}
	}


	if (m_ChangesFog)
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AExponentialHeightFog::StaticClass(), FoundActors);
		MapExponentialHeightFog = Cast<AExponentialHeightFog>(FoundActors[0]);

		if (MapExponentialHeightFog)
		{
			DustyUtils::PrintMessage("Found");
		}
	}
}

void APlanetsKeyController::Tick(float DeltaTime)
{

	// floating effect that ocurrs after platform has reached location
	if (ReachedTarget && m_ActiveLittleFlotation)
	{
		m_FloatingTimer += DeltaTime;
		const FVector LittleMovement = DustyUtils::GetRandomMovement(FinalPlatformPosition, m_FloatingTimer, m_Amplitude, m_Frequency, 4);
		SetActorLocation(LittleMovement);
	}

	// if not reached yet
	if (!ReachedTarget && Activated)
	{
		CurrentSpeed = m_PlatformBaseSpeed;
		
		FVector CurrentLocation = GetActorLocation();
		const float CurrentDistance = FVector::Dist(CurrentLocation, FinalPlatformPosition);
		const float NormalizedCurrentDistance = CurrentDistance / MaxDistance;

		if (m_SpeedCurve)
		{
			CurrentSpeed *= m_SpeedCurve->GetFloatValue(NormalizedCurrentDistance);
		}

		// lerp to target
		ReachedTarget = ADustyCameraManager::CameraLerp(DeltaTime, CurrentLocation, FinalPlatformPosition, CurrentSpeed);
		SetActorLocation(CurrentLocation);
		
		control = false;
		// welcome to chaos
		if (ReachedTarget)
		{
			if(m_platformSound)
			{
				GetGameInstance()->GetSubsystem<UAudioSubsytem>()->StopMetasound(m_platformSound);
			}
			
			// if there was a camera running up to this point but there is no planetary camera
			if (m_InitCameraOnPoleInserted && m_EventCamera && !m_BasePlanetaryCamera)
			{
				// call for exit and set base values
				m_EventCamera->Exit();

				// enable player to get out of pole state and dont allow usage since it is once use only (elevator)
				DustyCharacter->CanUnnailPole = true;
				Unlocked = false;
				DustyCharacter->ActionsFilterComponent->SetCurrentState(UGroundMovementState::StaticClass());
			}
			// if there was a camera running and there is also a planetary camera
			else if(m_InitCameraOnPoleInserted && m_EventCamera && m_BasePlanetaryCamera)
			{
				m_EventCamera->Exit();
				m_ActiveLittleFlotation = true;
				// enable player tu Exit pole state but dont do it (let the cameras control the input)
				DustyCharacter->CanUnnailPole = true;
			}
			// if there was no initial camera && there is a event camera and also a planetary camera
			else if(!m_InitCameraOnPoleInserted && m_EventCamera && m_BasePlanetaryCamera)
			{
				m_EventCamera->Init();
				m_ActiveLittleFlotation = true;
				// enable player tu Exit pole state but dont do it (let the cameras control the input)
				DustyCharacter->CanUnnailPole = true;
			}
			// if there was no initial camera && there is a event camera and there is no planetary camera
			else if(!m_InitCameraOnPoleInserted && m_EventCamera && !m_BasePlanetaryCamera)
			{
				m_EventCamera->Init();
				m_ActiveLittleFlotation = true;

				// enable player to get out of pole state and dont allow usage since it is once use only (elevator)
				DustyCharacter->CanUnnailPole = true;
				Unlocked = false;
				DustyCharacter->ActionsFilterComponent->SetCurrentState(UGroundMovementState::StaticClass());
			}
			// only for planetary usage
			else if (m_BasePlanetaryCamera)
			{
				m_BasePlanetaryCamera->Init();
				// enable player tu Exit pole state but dont do it (let the planetary camera control the input)
				DustyCharacter->CanUnnailPole = true;
			}
			else // if there are no cameras
			{
				// exit player from pole control and dont allow usage again
				DustyCharacter->CanUnnailPole = true;
				Unlocked = false;
				DustyCharacter->ActionsFilterComponent->SetCurrentState(UGroundMovementState::StaticClass());
			}
			Activated = false;
			control = true;
		}
	}
}

void APlanetsKeyController::Update(float _DeltaTime, FVector2D _Input)
{
	if (control)
	{
		if (!controlPlanets)
		{
			if(RayPlanets.Num() > 0 && m_OrbitMeshes.Num() > 0)
			{
				if (m_CanReceiveVerticalInput && abs(_Input.Y) > m_VerticalInputThreshold)
				{
					if (_Input.Y > 0) // Going up
					{
						m_OrbitMeshes[m_CurrentDiscIdx]->SetActorHiddenInGame(true);
						RayPlanets[m_CurrentDiscIdx]->SetOutline(false);
						RayPlanets[DustyUtils::ModClamp(m_CurrentDiscIdx + 1, 0, MAX_DISCS - 1)]->SetOutline(true);
						m_OrbitMeshes[DustyUtils::ModClamp(m_CurrentDiscIdx + 1, 0, MAX_DISCS - 1)]->SetActorHiddenInGame(false);
					}
					else if (_Input.Y < 0) // Going down
					{
						m_OrbitMeshes[m_CurrentDiscIdx]->SetActorHiddenInGame(true);
						RayPlanets[m_CurrentDiscIdx]->SetOutline(false);
						RayPlanets[DustyUtils::ModClamp(m_CurrentDiscIdx - 1, 0, MAX_DISCS - 1)]->SetOutline(true);
						m_OrbitMeshes[DustyUtils::ModClamp(m_CurrentDiscIdx - 1, 0, MAX_DISCS - 1)]->SetActorHiddenInGame(false);
					}
				}
				Super::Update(_DeltaTime, _Input);
				//RayPlanets[m_CurrentDiscIdx]->SetOutline(true);

				// Rotate discs
				if (_Input.X > m_HorizontalInputThreshold) // Rotating right
				{
					RayPlanets[m_CurrentDiscIdx]->TranslatePlanet(true);
				}
				else if (_Input.X < -m_HorizontalInputThreshold) // Rotating left
				{
					RayPlanets[m_CurrentDiscIdx]->TranslatePlanet(false);
				}
			}
			

		}

		else
		{
			if(RayPlanets.Num() > 0)
			{
				// Rotate discs
				if (_Input.X > m_HorizontalInputThreshold) // Rotating right
				{
					RayPlanets[m_CurrentDiscIdx]->RotatePlanet(true);
				}
				else if (_Input.X < -m_HorizontalInputThreshold) // Rotating left
				{
					RayPlanets[m_CurrentDiscIdx]->RotatePlanet(false);
				}
			}
		}
	}
}

void APlanetsKeyController::ChangeMode()
{
	controlPlanets = !controlPlanets;
}

void APlanetsKeyController::CameraSetted()
{
	DustyCharacter->CanUnnailPole = true;

	control = true;
	DustyUtils::ShowHud(GetWorld(), EHudCanvas::PlanetsButtons);
	if (NeedsToRotatePlanets)
	{
		ChangePlanetsTargets();
		NeedsToRotatePlanets = false;
		control = false;
	}
	if (RayPlanets.Num() > 0)
	{
		RayPlanets[m_CurrentDiscIdx]->SetOutline(true);
	}
	if (m_OrbitMeshes.Num() > 0)
	{
		m_OrbitMeshes[m_CurrentDiscIdx]->SetActorHiddenInGame(false);
	}


}

void APlanetsKeyController::CameraInFade(bool _Start)
{
	if (_Start)
	{
		if (MapExponentialHeightFog)
		{
			if (UExponentialHeightFogComponent* Fog = MapExponentialHeightFog->GetComponent())
			{
				DustyUtils::PrintMessage("MIAU");
				m_PreviousFogDensity = Fog->FogDensity;
				Fog->FogDensity = 0;

				Fog->MarkRenderStateDirty();
			}
		}
	}
	else
	{
		if (MapExponentialHeightFog)
		{
			if (UExponentialHeightFogComponent* Fog = MapExponentialHeightFog->GetComponent())
			{
				DustyUtils::PrintMessage("MIAUn't");
				Fog->FogDensity = m_PreviousFogDensity;

				Fog->MarkRenderStateDirty();
			}
		}
	}
}

void APlanetsKeyController::SetCamera(AEventCamera* _newCamera)
{
	m_EventCamera = _newCamera;
}


void APlanetsKeyController::ChangePlanetsTargets()
{
	for(ARayPlanet* planet : RayPlanets)
	{
		planet->RotatePuzzleCompleted();
	}
}

void APlanetsKeyController::OnPoleRemoved()
{
	Super::OnPoleRemoved();
	if (m_BasePlanetaryCamera)
	{
		if (RayPlanets.Num() > 0)
		{
			RayPlanets[m_CurrentDiscIdx]->SetOutline(false);
			control = false;
			m_BasePlanetaryCamera->Exit();
		}

		if (m_OrbitMeshes.Num() > 0)
		{
			m_OrbitMeshes[m_CurrentDiscIdx]->SetActorHiddenInGame(true);
		}


		DustyUtils::HideHud(GetWorld(), EHudCanvas::PlanetsButtons);
		
	}
	DustyCharacter->DustyInPlanetaryKeyController = false;
}

void APlanetsKeyController::OnPoleInserted()
{
	Super::OnPoleInserted();
	Activated = true;

	if(m_platformSound)
	{
		GetGameInstance()->GetSubsystem<UAudioSubsytem>()->PlayMetasound(m_platformSound, FVector::ZeroVector);
	}
	// if already on target and there is a planetary camera
	if (ReachedTarget && m_BasePlanetaryCamera)
	{
		m_BasePlanetaryCamera->Init();
		
	}
	// if a camera must be activated on pole inserted
	else if(m_InitCameraOnPoleInserted && m_EventCamera)
	{
		m_EventCamera->Init();
	}
	
	DustyCharacter->CanUnnailPole = false;
	DustyCharacter->DustyInPlanetaryKeyController = true;
}

void APlanetsKeyController::LoadActor()
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
	Ar << NeedsToRotatePlanets;
	Ar << ReachedTarget;
	Ar << Unlocked;


	if (NeedsToRotatePlanets)
	{
		ChangePlanetsTargets();
		NeedsToRotatePlanets = false;
		control = false;
	}
	
	UpdateMaterials();
}

void APlanetsKeyController::OnActorSaved_Implementation()
{
	FActorSaveData ActorData;

	ActorData.ActorName = GetFName();
	ActorData.LevelName = GetLevel()->GetOuter()->GetFName();
	ActorData.Transform = GetActorTransform();
	FMemoryWriter MemWriter(ActorData.ByteData);

	FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
	Ar.ArIsSaveGame = true;
	// Convert binary array back into actor's variables
	Ar << NeedsToRotatePlanets;
	Ar << ReachedTarget;
	Ar << Unlocked;

	GetGameInstance<UDustyGameInstance>()->AddActorSaveData(ActorData);
}
