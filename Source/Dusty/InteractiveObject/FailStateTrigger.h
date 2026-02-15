#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FailStateTrigger.generated.h"

class UBoxComponent;

UCLASS()
class DUSTY_API AFailStateTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	AFailStateTrigger();

private:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	UPROPERTY(EditAnywhere)
	UBoxComponent* Trigger;
};
