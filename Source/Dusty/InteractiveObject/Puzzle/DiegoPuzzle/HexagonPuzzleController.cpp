#include "HexagonPuzzleController.h"
#include "HexagonPuzzlePiece.h"
#include "Dusty/SaveGame/DustySaveGame.h"
#include "Dusty/DustyGameInstance.h"
#include "Dusty/Audio/AudioSubsytem.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "Dusty/Utils/DustyUtils.h"


void AHexagonPuzzleController::LoadActor()
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
	Ar << m_Index;
	Ar << m_ResetControl;
}

void AHexagonPuzzleController::OnActorSaved_Implementation()
{
	FActorSaveData ActorData;

	ActorData.ActorName = GetFName();
	ActorData.LevelName = GetLevel()->GetOuter()->GetFName();
	ActorData.Transform = GetActorTransform();
	FMemoryWriter MemWriter(ActorData.ByteData);

	FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
	Ar.ArIsSaveGame = true;
	// Convert binary array back into actor's variables
	Ar << m_Index;
	Ar << m_ResetControl;

	GetGameInstance<UDustyGameInstance>()->AddActorSaveData(ActorData);
}

AHexagonPuzzleController::AHexagonPuzzleController()
{
	PrimaryActorTick.bCanEverTick = false;
}


void AHexagonPuzzleController::BeginPlay()
{
	Super::BeginPlay();

	LoadActor();
	// erase all empty spaces in case design did things...
	m_Pieces.RemoveAll([](AHexagonPuzzlePiece* Element) { return Element == nullptr; });
	
	for (int i = 0; i < m_Pieces.Num(); ++i)
	{
		// piece setter of values for time and height control
		m_Pieces[i]->PieceBeginPlaySetter(
			this,
			m_HeightOfPuzzle / m_AmountOfLevels,
			m_BasePillarSpeed,
			m_RandomVariationPillarSpeed,
			m_DescentMultiplier);
	}
}

void AHexagonPuzzleController::UpdatePuzzle(APuzzlePiece* TriggerActor)
{
	if (AHexagonPuzzlePiece* TriggeredPiece = Cast<AHexagonPuzzlePiece>(TriggerActor))
	{
		// if pressed the current or superior level of puzzle
		if (TriggeredPiece->Index >= m_Index)
		{
			m_Status = TriggeredPiece->CorrectPillar;

			DustyUtils::PrintMessage("m_Status", m_Status);
			// base cases start
			// if wrong pillar and index is Zero then ignore
			if (!m_Status && m_Index <= 0)
			{
				return;
			}

			if(!m_Status && m_Index == 15)
			{
				return;
			}
			// if reset control is active and index is checkpoint then ignore
			if (m_ResetControl && !m_Status && m_Index == m_CheckPointIndex)
			{
				return;
			}
			// base cases end

			if(m_Status && m_Index != TriggeredPiece->Index)
			{
				return;
			}

			DustyUtils::PrintMessage("m_CheckPointIndex", m_CheckPointIndex);

			//if it's a correct piece and reset control is inactive
			if (m_Status && !m_ResetControl)
			{
				// if it is the checkpoint index
				if (m_Index == m_CheckPointIndex)
				{
					DustyUtils::PrintMessage("ResetControl", m_ResetControl);
					m_ResetControl = true;
				}
			}
			
			// check if pillar is the current needed pillar
			if (m_Status && TriggeredPiece->Index == m_Index)
			{
				++m_Index;
				DustyUtils::PrintMessage("m_AmountOfLevels", m_AmountOfLevels);
				
				//Play hexagon sound
				UAudioSubsytem* audioSubsystem = GetGameInstance()->GetSubsystem<UAudioSubsytem>();
				if(audioSubsystem && m_sound)
				{
					audioSubsystem->PlaySound3D(m_sound, GetActorLocation());
				}
				
				// if puzzle at the top or at the checkpoint then activate piece
				if (m_Index == m_AmountOfLevels || m_Index-1 == m_CheckPointIndex)
				{
					TriggeredPiece->Activate(true);
				}

			}
			else // if wrong pillar
			{

				DustyUtils::PrintMessage("ResetControl Wrong", m_ResetControl);
				// if reset control is active then reset index is check point index
				if(m_ResetControl)
				{
					m_Index = m_CheckPointIndex + 1;
					DustyUtils::PrintMessage("m_Index Wrong", m_Index);
				}
				// else it is Zero so the level resets to base
				else
				{
					m_Index = 0;

				}
				m_Status = false;
			}
			
			//Play hexagon sound
			UAudioSubsytem* audioSubsystem = GetGameInstance()->GetSubsystem<UAudioSubsytem>();
			if(audioSubsystem && m_sound)
			{
				audioSubsystem->PlaySound3D(m_sound, GetActorLocation());
			}
			for (int i = 0; i < m_Pieces.Num(); ++i)
			{
				if (m_Pieces[i]->Index >= m_Index)
				{
					m_Pieces[i]->Activate(m_Status);
				}
			}
		}
	}
}

int AHexagonPuzzleController::GetCurrentIndex() const
{
	return m_Index;
}

bool AHexagonPuzzleController::GetCurrentStatus() const
{
	return m_Status;
}
