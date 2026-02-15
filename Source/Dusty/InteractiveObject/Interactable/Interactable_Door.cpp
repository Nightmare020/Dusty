#include "Interactable_Door.h"
#include "../../Character/DustyCharacter.h"

AInteractable_Door::AInteractable_Door()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);

}

void AInteractable_Door::Interact()
{
	//UE_LOG(LogTemp, Warning, TEXT("DOOR OPENED"));
}
