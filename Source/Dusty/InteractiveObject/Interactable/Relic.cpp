#include "Relic.h"

#include "Dusty/DustyGameInstance.h"

ARelic::ARelic()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("mesh");
	SetRootComponent(Mesh);
}

void ARelic::Interact()
{
	if (!IsEnabled)
	{
		UDustyGameInstance* dustyGameInstance = GetWorld()->GetGameInstance<UDustyGameInstance>();
		Mesh->SetVisibility(false);
		Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);;
		IsEnabled = true;
	}
}

