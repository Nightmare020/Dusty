#pragma once

#include "CoreMinimal.h"
#include "BaseActors/MazeActorTrigger.h"
#include "MazeTriggerLighter.generated.h"


UCLASS()
class DUSTY_API AMazeTriggerLighter : public AMazeActorTrigger
{
private:
	GENERATED_BODY()

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "LightsTarget")
	bool m_LightsTarget = true;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "OneUseOnly")
	bool m_OneUseOnly = true;
	
public:
	virtual void DoBeginOverlap(AActor* OtherActor) override;
	
};
