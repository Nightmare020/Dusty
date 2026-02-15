#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../ToggleActor.h"
#include "LightActor.generated.h"


class AMazeTriggerBlockMovement;

UCLASS()
class DUSTY_API ALightActor : public AToggleActor
{
	GENERATED_BODY()
	
public:	
	ALightActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void Activate(bool Activating) override;
	virtual void Activate(bool _bActivation, AActor* _actor) override;

	UPROPERTY(EditAnywhere, Category = "Design | Light")
		bool Lighted = false;

	virtual void LoadActor();

	virtual void OnActorSaved_Implementation();

	virtual bool GetLightStatus();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design")
	AMazeTriggerBlockMovement* MazeNPC_TargetActor = nullptr;
};
