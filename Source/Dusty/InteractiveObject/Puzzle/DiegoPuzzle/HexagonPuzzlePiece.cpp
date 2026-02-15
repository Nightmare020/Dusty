#include "HexagonPuzzlePiece.h"
#include "HexagonPuzzleController.h"
#include "Dusty/Character/DustyCharacter.h"
#include "Dusty/SaveGame/DustySaveGame.h"
#include "Dusty/DustyGameInstance.h"


AHexagonPuzzlePiece::AHexagonPuzzlePiece()
{
	PrimaryActorTick.bCanEverTick = true;
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("Root"));

	HexagonTrigger = CreateDefaultSubobject<UStaticMeshComponent>("HexagonTrigger");
	HexagonTrigger->SetupAttachment(RootComponent);

	Pillar = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Pillar->SetupAttachment(RootComponent);

	TopScene = CreateDefaultSubobject<USceneComponent>("Top");
	//TopScene->SetupAttachment(Pillar);
}


void AHexagonPuzzlePiece::BeginPlay()
{
	Super::BeginPlay();

	HexagonTrigger->OnComponentBeginOverlap.AddDynamic(this, &AHexagonPuzzlePiece::TriggerBeginOverlap);
	
	m_MinHeight = GetActorLocation().Z;
	LoadActor();
	if (!m_LightTilt)
	{
		SetActorTickEnabled(false);
	}
	if (m_InitialLight)
	{
		DustyUtils::SetEmissiveMaterialValues(Pillar, m_MaxLightIntensityRed, m_MaxLightIntensityGreen, m_MaxLightIntensityBlue);
	}
}


void AHexagonPuzzlePiece::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_Updating)
	{
		FVector CurrentLocation = GetActorLocation();

		// lerp from current to target using speed and deltatime
		float const PillarDistance = abs(m_NewHeight-CurrentLocation.Z);
		float const FAlpha = FMath::Min(DeltaTime * m_CurrentBlendSpeed / PillarDistance, 1.0f);
		float const NewScale = FMath::Lerp(CurrentLocation.Z, m_NewHeight, FAlpha);
		
		CurrentLocation.Z = NewScale;

		if (NewScale > MAX_flt)
		{
			UE_LOG(LogTemp, Error, TEXT("Index of Puzzle controller %s was set wrongly"), *PuzzleController->GetName());
		}
		else
		{
			SetActorLocation(CurrentLocation);
		}

		// if current and target are the same we have reached destination
		if (CurrentLocation.Z == m_NewHeight)
		{
			m_Updating = false;
			if (!m_LightTilt)
			{
				SetActorTickEnabled(false);
			}
		}
	}

	// light tilt logic (always ticking)
	if (m_LightTilt)
	{
		FVector Color = FVector(m_MaxLightIntensityRed, m_MaxLightIntensityGreen, m_MaxLightIntensityBlue);
		m_ColorTimer += DeltaTime * FMath::FRandRange(m_MinRandom, m_MaxRandom);
		FVector const RandomVector = DustyUtils::GetRandomMovement(FVector(0,0,0), m_ColorTimer, m_Amplitude, m_Frequency, 3);
		Color *= RandomVector.Size();

		DustyUtils::SetEmissiveMaterialValues(Pillar, Color.X, Color.Y, Color.Z);

	}
}


void AHexagonPuzzlePiece::TriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<ADustyCharacter>(OtherActor) && PuzzleController)
	{
		// check if its really colliding (maybe pole)
		FVector StartLocation = OtherActor->GetActorLocation();
		FVector EndLocation = StartLocation - FVector(0, 0, 1000);
		FCollisionQueryParams TraceParams(FName(TEXT("RayTrace")), true);
		TraceParams.AddIgnoredActor(OtherActor);  // Ignore dusty

		FHitResult HitResult;
		if(bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, TraceParams))
		{
			// if the hit is with this actor then update puzzle
			if (this == HitResult.GetActor())
			{
				PuzzleController->UpdatePuzzle(this);
			}
		}
	}
}


void AHexagonPuzzlePiece::Activate(bool _Activating)
{
	if (m_Controller)
	{
		// enable tick for movement
		SetActorTickEnabled(true);

		// max allowed new height for puzzle piece
		float MaxNewHeight = m_HeightChange * m_Controller->GetCurrentIndex();

		// puzzle is growing
		if (m_Controller->GetCurrentStatus())
		{
			m_CurrentBlendSpeed = m_BaseBlendSpeed;
		}
		else // puzzle is resetting
		{
			m_CurrentBlendSpeed = m_BaseBlendSpeed * m_DescentMultiplier;
		}
		
		// only not apply random to the just selected pillar and when activate is true
		if (!(CorrectPillar && (m_Controller->GetCurrentIndex() - 1) == Index) && _Activating)
		{
			// not apply when target index is reset index
			if (m_Controller->GetCurrentIndex() != 0)
			{
				MaxNewHeight += FMath::RandRange(0.f, m_Controller->GetRandomVariationInHeightOfPuzzle());
			}
		}

		m_NewHeight = m_MinHeight + MaxNewHeight;
		m_Updating = true;
	}
}


void AHexagonPuzzlePiece::PieceBeginPlaySetter(AHexagonPuzzleController* _PuzzleController, float _NewHeightChange, float _BlendSpeed, float _RandomBlendSpeedVariation, float _DescentMultiplier)
{
	PuzzleController = _PuzzleController;
	m_Controller = _PuzzleController;
	m_HeightChange = _NewHeightChange;

	// speed is always the same but it has extra randomness HEHE
	m_RandomBlendSpeedVariation = _RandomBlendSpeedVariation;
	m_BaseBlendSpeed =_BlendSpeed + FMath::RandRange(0.f,_RandomBlendSpeedVariation);
	m_DescentMultiplier = _DescentMultiplier;
}

void AHexagonPuzzlePiece::SetCurrentHeightAsBase()
{
	m_MinHeight = GetActorLocation().Z;
}


void AHexagonPuzzlePiece::ApplyTexture(UMaterialInterface* Material)
{
	Pillar->SetMaterial(0, Material);
}

void AHexagonPuzzlePiece::ToggleLighted(bool _newValue)
{
	m_InitialLight = _newValue;
	if (m_InitialLight)
	{
		DustyUtils::SetEmissiveMaterialValues(Pillar, m_MaxLightIntensityRed, m_MaxLightIntensityGreen, m_MaxLightIntensityBlue);
	}
	else
	{
		DustyUtils::SetEmissiveMaterialValues(Pillar, 0, 0, 0);
	}
	
}

void AHexagonPuzzlePiece::LoadActor()
{
	FActorSaveData ActorSaveData = GetGameInstance<UDustyGameInstance>()->GetActorSaveData(GetFName(), GetLevel()->GetOuter()->GetFName());

	if (ActorSaveData.Transform.GetLocation() == FVector::ZeroVector)
	{
		return;
	}
	SetActorTransform(ActorSaveData.Transform);

}

void AHexagonPuzzlePiece::OnActorSaved_Implementation()
{
	FActorSaveData ActorData;

	ActorData.ActorName = GetFName();
	ActorData.LevelName = GetLevel()->GetOuter()->GetFName();
	ActorData.Transform = GetActorTransform();

	GetGameInstance<UDustyGameInstance>()->AddActorSaveData(ActorData);

}
