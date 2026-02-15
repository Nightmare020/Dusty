#include "FlowerDetector.h"

#include "FlowerNPC.h"
#include "Dusty/Character/DustyCharacter.h"

AFlowerDetector::AFlowerDetector()
{
	PrimaryActorTick.bCanEverTick = false;

	m_TestBox = CreateDefaultSubobject<UStaticMeshComponent>("BoxComponent");
	m_TestBox->SetupAttachment(RootComponent);
}

void AFlowerDetector::BeginPlay()
{
	Super::BeginPlay();
}

void AFlowerDetector::Init(ADustyCharacter* _player)
{
	m_Player = _player;
}

float AFlowerDetector::CheckForPlayer(float DetectionDistance)
{
	if(m_Player)
	{
		// check if player is on sight
		FHitResult OutHit;
		m_TargetLocation = m_Player->GetActorLocation();

		float const DistanceToTarget = FVector::Dist(m_TargetLocation, m_TraceStartLocation);

		// if is close enough
		if (DistanceToTarget <= DetectionDistance)
		{
			// line trace to the player to see if it hits
			//DrawDebugLine(GetWorld(), m_TraceStartLocation, m_TargetLocation, FColor::Red, false, -1.f, 0, 5.0f);
			if (GetWorld()->LineTraceSingleByChannel(OutHit, m_TraceStartLocation, m_TargetLocation, ECollisionChannel::ECC_Visibility))
			{
				if (OutHit.GetActor()->IsA(ADustyCharacter::StaticClass()))
				{
					return DistanceToTarget; // return distance to check
				}
			}
		}
	}
	return -1; // didn't detect anything
}

#pragma region Setters / Getters

void AFlowerDetector::SetOrderedFlowers(const TArray<AFlowerNPC*>& _flowers)
{
	m_Flowers = _flowers; // copy the array to the Detector array
	float m_MaxDistance = -1;
	float m_MinDistance = MAX_FLT;

	AFlowerNPC* TempFarthest = nullptr;
	AFlowerNPC* TempClosest = nullptr;

	m_TraceStartLocation = GetActorLocation();
	
	for (AFlowerNPC* Flower : m_Flowers)
	{
		if (Flower)
		{
			const float TempDistance = FVector::Dist(m_TraceStartLocation, Flower->GetActorLocation());
		
			// if it is the farthest one yet to detector
			if (TempDistance > m_MaxDistance)
			{
				m_MaxDistance = TempDistance;
				TempFarthest = Flower;
			}

			// if it is the closest one yet to detector
			if (TempDistance < m_MinDistance)
			{
				m_MinDistance = TempDistance;
				TempClosest = Flower;
			}
		}
	}

	// set now farthest and closest locations
	if (TempFarthest && TempClosest)
	{
		m_ClosestFlowerLocation = TempClosest->GetActorLocation();
		m_FarthestFlowerLocation = TempFarthest->GetActorLocation();
	}
	
}

void AFlowerDetector::SetFlowersMode(FlowerNPCMode _newMode)
{
	if (m_Flowers.Num() >= 1)
	{
		// get max distance for normalization
		const float MaxDistance = FVector::Dist(m_ClosestFlowerLocation, m_FarthestFlowerLocation);
		for (AFlowerNPC* Flower : m_Flowers)
		{
			if (Flower)
			{
				// set normalized time values for reaction time
				FVector FlowerLocation = Flower->GetActorLocation();
				const float CurrentDistance = FVector::Dist(m_ClosestFlowerLocation, FlowerLocation);
				const float NormalizedDistance = CurrentDistance/MaxDistance;
				const float TimeNeeded = NormalizedDistance * m_ReactionTime;
				
				Flower->SetMode(_newMode, TimeNeeded);
			}
		}
	}
}

void AFlowerDetector::SetReactionTime(float _reactionTime)
{
	m_ReactionTime = _reactionTime;
}

#pragma endregion