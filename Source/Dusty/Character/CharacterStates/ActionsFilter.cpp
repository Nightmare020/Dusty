#include "ActionsFilter.h"
#include "../DustyCharacter.h"
#include "../../Cinematics/CinematicsSubsystem.h"
#include "../../Utils/DustyUtils.h"

UActionsFilter::UActionsFilter()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UActionsFilter::BeginPlay()
{
	Super::BeginPlay();

	if (!StatesDataAsset)
	{
		//DustyUtils::PrintMessage("---No data asset associated with the 'ActionFilter' component---", FVector2D(1.5f, 1.5f), FColor::Red, 5);
		return;
	}
	m_DustyCharacter = Cast<ADustyCharacter>(GetOwner());

	// Pair is of type TSubclassOf<UBaseState>, FAvailableStates>
	for (const auto& Pair : StatesDataAsset->StatePriorityMap)
	{
		if (UBaseState* NewStateInstance = NewObject<UBaseState>(this, Pair.Key))
		{
			m_statesInstancesMap.Add(Pair.Key, NewStateInstance);
			NewStateInstance->SetOwner(m_DustyCharacter);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to create instance of state class."));
		}
	}

	// Get a cinematic subsystem reference
	UCinematicsSubsystem* cinematicSubsystem = GetOwner()->GetGameInstance()->GetSubsystem<UCinematicsSubsystem>();

	//Initialize
	if (cinematicSubsystem->IsCinematicPlaying)
	{
		m_currentBaseState = *m_statesInstancesMap.Find(UCinematicState::StaticClass());
		m_currentBaseStateClass = UCinematicState::StaticClass();
		m_currentBaseState->Init();
	}
	else
	{
		m_currentBaseState = *m_statesInstancesMap.Find(UGroundMovementState::StaticClass());
		m_currentBaseStateClass = UGroundMovementState::StaticClass();
		m_currentBaseState->Init();
	}
}


void UActionsFilter::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (m_currentBaseState != nullptr && m_currentBaseState->CanUpdateTick)
	{
		m_currentBaseState->Update(DeltaTime);
	}

	if (m_currentBaseState != nullptr)
	{
		//DustyUtils::DustyDrawDebugString(GetWorld(), FVector(0, 0, 0), m_currentBaseState->GetFName().ToString(), m_DustyCharacter, FColor::White, 0.0f);
	}
}


bool UActionsFilter::IsStateAvailable(const TSubclassOf<UBaseState> _state)
{
	bool StateAvailable = false;
	if (!StatesDataAsset)
	{
		//DustyUtils::PrintMessage("---NO DATA ASSET ASSOCIATED WITH THE 'ActionFilter' COMPONENT---", FVector2D(1.5f, 1.5f), FColor::Red, 5);
		return false;
	}

	for (const auto& Pair : StatesDataAsset->StatePriorityMap)
	{
		if (Pair.Key == m_currentBaseStateClass)
		{
			for (const auto& AvailableState : Pair.Value.AvailableStates)
			{
				if (AvailableState == _state)
				{
					return true;
				}
			}
		}
	}
	return false;
}


void UActionsFilter::SetCurrentState(TSubclassOf<UBaseState> _newState)
{
 	if (!m_statesInstancesMap.Contains(_newState))
	{
		//DustyUtils::PrintMessage("The state doesn't exist", FColor::Red, 5);
		return;
	}
	
	UBaseState* NewState = *m_statesInstancesMap.Find(_newState);

	if (m_currentBaseState != nullptr && NewState != nullptr)
	{
		if (IsStateAvailable(_newState))
		{
			m_currentBaseState->Exit();

			m_currentBaseState = NewState;
			m_currentBaseStateClass = _newState;

			NewState->Init();
		}
		else
		{
			//DustyUtils::PrintMessage("State not available", FColor::Red, 5);
		}
	}
	else
	{
		//DustyUtils::PrintMessage("Can't change state", FColor::Red, 5);
	}
}


UBaseState* UActionsFilter::GetCurrentBaseState()
{
	return m_currentBaseState;
}


TSubclassOf<UBaseState> UActionsFilter::GetCurrentBaseStateClass()
{
	return m_currentBaseStateClass;
}
