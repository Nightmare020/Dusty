#pragma once

#include "CoreMinimal.h"
#include "../Light/LightActor.h"

#include "InteractiveRoot.generated.h"

class UBoxComponent;
class USplineMeshComponent;

UCLASS()
class DUSTY_API AInteractiveRoot : public AActor
{
	GENERATED_BODY()

public:	
	AInteractiveRoot();

	virtual void BeginPlay() override;

	void UpdateRoots(float _Value);

protected:

	UPROPERTY(BlueprintReadWrite)
	TArray<USplineMeshComponent*> RootsMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline", meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* SplineMaterial_1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline", meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* SplineMaterial_2;	

	UPROPERTY(Transient)
	UMaterialInstanceDynamic* DynamicMaterial;
};
