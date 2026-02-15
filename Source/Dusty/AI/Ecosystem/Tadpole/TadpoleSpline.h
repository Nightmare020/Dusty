#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TadpoleSpline.generated.h"

class USplineComponent;

UCLASS()
class DUSTY_API ATadpoleSpline : public AActor
{
	GENERATED_BODY()
	
public:	
	ATadpoleSpline();
	
	virtual void BeginPlay() override;
	
	FVector GetNextLocation(const FVector& CurrentLocation, float DistanceToMove) const;
	FVector GetClosestLocation(const FVector& CurrentLocation, float& Direction) const;
	FVector GetClosestLocation(const float SplineLocation) const;

	FRotator GetCurrentRotation(const FVector& CurrentLocation) const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "Spline")
	USplineComponent* m_Spline = nullptr;
};
