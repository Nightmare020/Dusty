#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TadpoleNPC.generated.h"

class ATadpoleChillZone;
class ATadpoleSpline;
class ATadpoleController;
class ATadpoleAIController;

UCLASS()
class DUSTY_API ATadpoleNPC : public ACharacter
{
	GENERATED_BODY()

public:
	ATadpoleNPC();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void SetController(ATadpoleController* _newController);
	void SetTargetLocation(const FVector& _newTargetLocation);

	FVector GetNextSplineTarget(float _predictionTime);
	FVector GetTargetSplineClosestLocation();
	
	bool HasAIController();

public:
	float SwimDirection = 1.f;

private:
	UPROPERTY(Transient)
	ATadpoleSpline* m_TargetSpline = nullptr;

	UPROPERTY(Transient)
	ATadpoleController* m_TadpoleController = nullptr;

	UPROPERTY(Transient)
	ATadpoleAIController* m_TadpoleAIController = nullptr;
	
};
