#include "FlowerController.h"

#include "FlowerDetector.h"
#include "FlowerNPC.h"
#include "Dusty/Camera/DustyCameraManager.h"
#include "Dusty/Character/DustyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AFlowerController::AFlowerController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AFlowerController::BeginPlay()
{
	Super::BeginPlay();
	m_CheckingRateTimer = m_CheckingRate;
	m_CurrentDistanceToActivate = m_DistanceToActivate;
	
	// clean nulls from array in case design trolled
	m_Detectors.RemoveAll([](const AFlowerDetector* Detector) { return Detector == nullptr; });
	m_Flowers.RemoveAll([](const AFlowerNPC* Flower) { return Flower == nullptr; });

	for (AFlowerDetector* Detector: m_Detectors)
	{
		if(Detector)
		{
			Detector->SetReactionTime(m_MaxReactionTime);
			Detector->SetOrderedFlowers(m_Flowers);
		}
	}
}

void AFlowerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(RefChecker())
	{
		for (AFlowerDetector* Detector: m_Detectors)
		{
			Detector->m_TestBox->SetMaterial(0, Detector->Materials[1]);
		}

		if (DetectionResponsible)
		{
			DetectionResponsible->m_TestBox->SetMaterial(0, DetectionResponsible->Materials[0]);
		}

		// checker for later flower rotation
		bool CanRotate = true;
		
		// first check if Controller is on cooldown
		m_CoolDownTimer -= DeltaTime;
		if (m_CoolDownTimer <= 0.f)
		{
			m_CheckingRateTimer -= DeltaTime;
			if (m_CheckingRateTimer <= 0.f)
			{
				if(DetectionResponsible) // if player has been detected this is the one that is checking now
				{
					// factor also applied so plants have extra time to change states
					m_CurrentDistanceToActivate = m_DistanceToActivate * m_DistanceFactor;
					float const NewDistance = DetectionResponsible->CheckForPlayer(m_CurrentDistanceToActivate);

					// distance to -1 means it didn't collide
					if(NewDistance < 0.f)
					{
						// current responsible has lost sight check the rest
						AFlowerDetector* TempResponsible = DetectionResponsible;
						DetectionResponsible = nullptr;
						
						CheckDetectors();

						if(!DetectionResponsible) // if it still is null then we need set the new flowers mode
						{
							TempResponsible->SetFlowersMode(FlowerNPCMode::Looking);
							m_CoolDownTimer = 0.f; // cooldown to zero because it needs to be ready to react to player instantly
						}
					}
				}
				else
				{
					// check all detectors
					CheckDetectors();

					// only the base distance because we dont need to give extra space for state swapping
					m_CurrentDistanceToActivate = m_DistanceToActivate;
					
					if(DetectionResponsible) // if it is not null then we need set the new flowers mode
					{
						DetectionResponsible->SetFlowersMode(FlowerNPCMode::Hiding);
						m_CoolDownTimer = m_CoolDown;
					}
				}
				m_CheckingRateTimer = m_CheckingRate;
			}
		}

		// test de look at player
		if(m_Player && m_Player->CameraManager)
		{
			const float CurrentDistance = FVector::Dist(GetActorLocation(), m_Player->GetActorLocation());
			if (CurrentDistance < m_DistanceToActivate * m_DistanceFactor)
			{
				CanRotate = false;
			}

			if (CanRotate)
			{
				for (AFlowerNPC* Flower: m_Flowers)
				{
					if(Flower)
					{
					
						FVector FlowerLocation = Flower->GetActorLocation();
						FRotator FlowerRotation = Flower->GetActorRotation();
					
						FVector TargetLocation = m_Player->GetActorLocation();

						FRotator TargetRotation = FRotator::ZeroRotator;
						TargetRotation.Yaw = UKismetMathLibrary::FindLookAtRotation(FlowerLocation, TargetLocation).Yaw;
					
						m_Player->CameraManager->CameraLerp(DeltaTime, FlowerRotation, TargetRotation, m_FlowerRotationSpeed);
					
						Flower->SetActorRotation(FlowerRotation);
					}
				}
			}
		}
	}
}

void AFlowerController::Init(ADustyCharacter* _player)
{
	if (_player)
	{
		m_Player = _player;
		for (AFlowerDetector* Detector : m_Detectors)
		{
			if (Detector)
			{
				Detector->Init(_player);		
			}
		}
	}
}

void AFlowerController::CheckDetectors()
{
	float Distance = MAX_FLT;
	for (AFlowerDetector* Detector : m_Detectors)
	{
		if (Detector)
		{
			float const NewDistance = Detector->CheckForPlayer(m_CurrentDistanceToActivate);
			if (NewDistance != -1.f)
			{
				// if this detector is closer than stored detector
				if(Distance > NewDistance)
				{
					// if this detector is closest to player then it's his responsibility now
					Distance = NewDistance;
					DetectionResponsible = Detector;
				}
			}
		}
	}
}

bool AFlowerController::RefChecker()
{
	if (!m_Player || !m_Player->CameraManager)
	{
		m_Player = Cast<ADustyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

		if(m_Player)
		{
			Init(m_Player);
		}
	}
	return m_Player &&  m_Player->CameraManager;
}
