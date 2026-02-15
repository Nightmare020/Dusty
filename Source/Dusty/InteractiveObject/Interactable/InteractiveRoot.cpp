#include "InteractiveRoot.h"
#include "../../Utils/DustyConst.h"
#include "../../Utils/DustyUtils.h"
#include "Components/BoxComponent.h"
#include "Components/SplineMeshComponent.h"

AInteractiveRoot::AInteractiveRoot()
{
	PrimaryActorTick.bCanEverTick = true;
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("Root"));
}

void AInteractiveRoot::UpdateRoots(float _Value)
{

	DynamicMaterial->SetScalarParameterValue(RootParam, _Value);	
}


void AInteractiveRoot::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);

	DynamicMaterial = UMaterialInstanceDynamic::Create(SplineMaterial_1, this);

	for (USplineMeshComponent* Mesh : RootsMesh)
	{
		if (Mesh)
		{
			Mesh->SetMaterial(0, DynamicMaterial);
		}
	}
}