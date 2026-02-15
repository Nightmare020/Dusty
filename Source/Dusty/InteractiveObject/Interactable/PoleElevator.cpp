// Fill out your copyright notice in the Description page of Project Settings.


#include "PoleElevator.h"

#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"
#include "../../Character/DustyCharacter.h"
#include "../../Character/CharacterStates/BaseState.h"
#include "../../Movement/PoleJumpConstraintActor.h"
#include "../../Character/CharacterStates/ActionsFilter.h"
#include "../../Character/DustyCharacter.h"
#include "../../Utils/DustyUtils.h"
#include "Components/PoseableMeshComponent.h"
#include "../../UI/HUD/HUDSubsystem.h"
#include "../../Character/DustyMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Dusty/Audio/AudioSubsytem.h"

APoleElevator::APoleElevator()
{
	PrimaryActorTick.bCanEverTick = false;
	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("Root")));

	m_ElevatorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	m_ElevatorMesh->SetupAttachment(RootComponent);

	m_ElevatorTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("ElevatorTrigger"));
	m_ElevatorTrigger->SetupAttachment(m_ElevatorMesh);

	PolePoint = CreateDefaultSubobject<USceneComponent>(TEXT("PolePoint"));
	PolePoint->SetupAttachment(m_ElevatorMesh);
}

void APoleElevator::BeginPlay()
{
	Super::BeginPlay();

	m_DistanceToCenter = (m_DustyPosition.VectorPlaneProject(m_DustyPosition, FVector::UpVector)).Size();
}


void APoleElevator::Interact()
{
	Super::Interact();


	if (!DustyCharacter->GetExtendiblePoleSkill()) //Unlocked skill?
	{
		return;
	}

	//DustyUtils::HideHud(GetWorld(), EHudCanvas::SingleButtonPrompt);

	FVector DustyLocation = DustyCharacter->GetActorLocation();
	DustyLocation.Z = GetActorLocation().Z;


	FVector DustyDirection = (DustyLocation - GetActorLocation()).GetSafeNormal();

	FVector Location = GetActorLocation() + DustyDirection * m_DistanceToCenter + FVector::UpVector * DustyCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	FRotator Rotation = (GetActorLocation() - Location).Rotation();

	DustyCharacter->CurrentPoleElevator = this;

	//First, set the character to the pole point
	DustyUtils::DustyMoveTo(DustyCharacter, Location, (-DustyDirection).Rotation(), &AInteractable::OnFinishedAutoMovement, this);


	
}

void APoleElevator::ExtendElevator()
{	
	//We set the state to pole jumping so we can fall
	DustyCharacter->ActionsFilterComponent->SetCurrentState(UPoleElevatorState::StaticClass());

}

void APoleElevator::ExitElevator()
{
	DustyCharacter->PoleJumpConstraintActor->Destroy();

	DustyCharacter->ActionsFilterComponent->SetCurrentState(UGroundMovementState::StaticClass());
	DustyCharacter->PoleSkeletal->SetHiddenInGame(false);

	/*if(DustyCharacter->m_trail2)
	{
		DustyCharacter->m_trail2->Deactivate();
	}*/
	
}

void APoleElevator::ClearTimers()
{
	GetWorld()->GetTimerManager().ClearTimer(m_TimeToEndAnimationHandle);
	GetWorld()->GetTimerManager().ClearTimer(m_RetractHandle);

}

void APoleElevator::PlayPoleSound()
{
	
}


void APoleElevator::Retract()
{
	UPoleElevatorState* PoleJumpState = Cast<UPoleElevatorState>(*DustyCharacter->ActionsFilterComponent->m_statesInstancesMap.Find(UPoleElevatorState::StaticClass()));
	bool CurrentExtended = PoleJumpState->Extended;
	if (DustyCharacter->PoleJumpConstraintActor && CurrentExtended)
	{
		DustyCharacter->DustyMovementComponent->Activate();
		DustyCharacter->PoleJumpConstraintActor->Extend(false);
		PoleJumpState->Extended = false;
		GetWorld()->GetTimerManager().SetTimer(m_TimeToEndAnimationHandle, this, &APoleElevator::ExitElevator, DustyCharacter->PoleJumpConstraintActor->GetExtensionTime(), false);
	}
}

void APoleElevator::OnFinishedAutoMovement()
{
	ExtendElevator();
}

void APoleElevator::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (ADustyCharacter* Character = Cast<ADustyCharacter>(OtherActor))
	{
		if (Character->ActionsFilterComponent->GetCurrentBaseStateClass() != UAutoMovementState::StaticClass())
		{
			DustyUtils::ShowHud(GetWorld(), EHudCanvas::SingleButtonPrompt, m_HudButtonId);
		}
	}
}

void APoleElevator::NotifyActorEndOverlap(AActor* OtherActor)
{
	if (OtherActor->IsA(ADustyCharacter::StaticClass()))
	{
		DustyUtils::HideHud(GetWorld(), EHudCanvas::SingleButtonPrompt);
	}
}

void APoleElevator::SpawnPoleElevator()
{
	const float DesiredScale = ((DustyCharacter->GetBottomPoleExtension() + DustyCharacter->GetPoleBaseHeight()) / 100);
	//Then spawn the pole jump constraint
	PoleJumpValuesStruct* PoleJumpValues = new PoleJumpValuesStruct(PolePoint->GetComponentLocation(), FVector::ZeroVector, -GetActorUpVector() * DustyCharacter->GetPoleBaseHeight(), m_TimeToExtendInPoleElevator, m_MaxPoleElevatorAngleBeforeRetract, m_PoleElevatorMovementFactor, 1, true, DesiredScale, EPoleMovementType::Elevator);
	DustyCharacter->SpawnPoleJumpConstraint(PoleJumpValues);

	DustyCharacter->PoleJumpConstraintActor->Extend(true);

	FTimerHandle StateHandle;
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &APoleElevator::SetExtendedInState, true);
	GetWorld()->GetTimerManager().SetTimer(StateHandle, TimerDelegate, DustyCharacter->PoleJumpConstraintActor->GetExtensionTime(), false);	

	GetWorld()->GetTimerManager().SetTimer(m_RetractHandle, this, &APoleElevator::Retract, m_TimeToRetract, false);

	GetWorld()->GetTimerManager().SetTimer(m_poleSoundTimer, this, &APoleElevator::PlayPoleSound, m_timeToPoleSound, false);
	if(m_poleSound)
	{
		GetGameInstance()->GetSubsystem<UAudioSubsytem>()->PlaySound2D(m_poleSound);
	}

}



void APoleElevator::SetExtendedInState(bool _Extended)
{
	UPoleElevatorState* PoleJumpState = Cast<UPoleElevatorState>(*DustyCharacter->ActionsFilterComponent->m_statesInstancesMap.Find(UPoleElevatorState::StaticClass()));
	PoleJumpState->Extended = _Extended;
}

