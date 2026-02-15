// Fill out your copyright notice in the Description page of Project Settings.


#include "KeyController.h"
#include "Components/SphereComponent.h"
#include "Disc.h"
#include "../ToggleActor.h"
#include "../../Character/DustyCharacter.h"
#include "../../UI/HUD/InteractiveButtonInterface.h"
#include "../../UI/HUD/HUDSubsystem.h"
#include "Dusty/SaveGame/DustySaveGame.h"
#include "Dusty/DustyGameInstance.h"
#include "Dusty/Utils/DustyConst.h"
#include "Components/StaticMeshComponent.h"


void AKeyController::EnableVerticalInputCallback()
{
	m_CanReceiveVerticalInput = true;
	GetWorldTimerManager().ClearTimer(m_VerticalInputTimerHandle);
}

void AKeyController::Update(float _DeltaTime, FVector2D _Input)
{	
	if (m_CanReceiveVerticalInput && abs(_Input.Y) > m_VerticalInputThreshold)
	{
		m_CanReceiveVerticalInput = false;
		GetWorldTimerManager().SetTimer(m_VerticalInputTimerHandle, this, &AKeyController::EnableVerticalInputCallback, m_VerticalInputCooldown, false);

		if (_Input.Y > 0) // Going up
		{
				m_PreviousDiscIdx = m_CurrentDiscIdx;
				m_CurrentDiscIdx = DustyUtils::ModClamp(m_CurrentDiscIdx + 1, 0, MAX_DISCS - 1);
		}
		else if (_Input.Y < 0) // Going down
		{
			m_PreviousDiscIdx = m_CurrentDiscIdx;
			m_CurrentDiscIdx = DustyUtils::ModClamp(m_CurrentDiscIdx - 1, 0, MAX_DISCS - 1);
		}
	}
}

//If you're reading this... peepo popee peepee po
