
#include "OpenTowerDoors.h"
#include "Components/SphereComponent.h"
#include "Dusty/Utils/DustyConst.h"
#include "../../UI/HUD/InteractiveButtonInterface.h"
#include "../../Character/DustyCharacter.h"
#include "../../Character/CharacterStates/ActionsFilter.h"
#include "../Interactable/DissolveDoor.h"
#include "Dusty/AI/Maze/Triggers/MazeTriggerTargetOverrider.h"

AOpenTowerDoors::AOpenTowerDoors()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("InteractionPlatform");
	Mesh->SetupAttachment(Root);

	m_KeyControllerTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("Key Controller Trigger"));
	m_KeyControllerTrigger->SetupAttachment(Mesh);

}

// Called when the game starts or when spawned
void AOpenTowerDoors::BeginPlay()
{
	Super::BeginPlay();

	DustyCharacter = DustyUtils::GetDustyCharacter();
}

// Called every frame
void AOpenTowerDoors::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (m_started)
	{
		if (m_currentTimeExitKeyAnimation <= TimeExitKeyAnimation)
		{
			m_currentTimeExitKeyAnimation += DeltaTime;
		}
		else
		{
			DustyCharacter->ActionsFilterComponent->SetCurrentState(UGroundMovementState::StaticClass());
			DustyCharacter->CanUnnailPole = true;
			m_started = false;
			m_Activated = false;
			DustyUtils::HideHud(GetWorld(), EHudCanvas::DiscButtons);
		}
	}
}

void AOpenTowerDoors::Activate(bool _bActivation)
{
	Super::Activate();
	m_Activated = _bActivation;
}

void AOpenTowerDoors::Deactivate()
{
	m_Activated = false;
}

void AOpenTowerDoors::NotifyActorBeginOverlap(AActor* OtherActor)
{
	// Check if the overlapping actor is Dusty
	ADustyCharacter* Character = Cast<ADustyCharacter>(OtherActor);

	if (Character)
	{
		DustyCharacter = Character;
		HasPlayerOverlapped = true;
		if (Character->ActionsFilterComponent->GetCurrentBaseStateClass() != UAutoMovementState::StaticClass())
		{
			DustyUtils::ShowHud(GetWorld(), EHudCanvas::SingleButtonPrompt, m_HudButtonId);
		}
	}


}

void AOpenTowerDoors::NotifyActorEndOverlap(AActor* OtherActor)
{
	if (OtherActor->IsA(ADustyCharacter::StaticClass()))
	{
		DustyUtils::HideHud(GetWorld(), EHudCanvas::SingleButtonPrompt);
	}
}

void AOpenTowerDoors::Interact()
{

	if (!m_Activated)
	{
		return;
	}

	Super::Interact();


	DustyUtils::HideHud(GetWorld(), EHudCanvas::SingleButtonPrompt);
	FVector Location = GetActorLocation() + m_DustyPosition;
	FRotator Rotation = (GetActorLocation() - Location).Rotation();


	//First, set the character to the pole point
	DustyUtils::DustyMoveTo(DustyCharacter, Location, Rotation, &AInteractable::OnFinishedAutoMovement, this);
}

void AOpenTowerDoors::OnFinishedAutoMovement()
{
	DustyCharacter->ActionsFilterComponent->SetCurrentState(UPoleKeyState::StaticClass());
	if (UPoleKeyState* State = Cast<UPoleKeyState>(DustyCharacter->ActionsFilterComponent->GetCurrentBaseState()))
	{
		State->SetupPoleKeyState();

		for (size_t i = 0; i < Doors.Num(); i++)
		{
			Doors[i]->Activate(true);
		}	

		if (TargetOverrider)
		{
			TargetOverrider->IsActive = true;
		}
	}

	m_started = true;
	m_Activated = false;
}
