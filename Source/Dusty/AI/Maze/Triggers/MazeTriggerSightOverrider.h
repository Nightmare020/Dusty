#pragma once

#include "CoreMinimal.h"
#include "BaseActors/MazeActorTrigger.h"
#include "MazeTriggerSightOverrider.generated.h"

class ADustyCharacter;

UCLASS()
class DUSTY_API AMazeTriggerSightOverrider : public AMazeActorTrigger
{
private:
	GENERATED_BODY()

private:
	UPROPERTY(Transient)
	ADustyCharacter* m_Player = nullptr;
	
public:
	virtual void BeginPlay() override;
	virtual void DoBeginOverlap(AActor* OtherActor) override;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design")
	bool NeedsToSeePlayer = true;
};
