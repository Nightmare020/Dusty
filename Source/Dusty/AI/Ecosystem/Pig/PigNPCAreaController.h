#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PigNPCAreaController.generated.h"

class APigNPC;
class APigNPCArea;

UCLASS()
class DUSTY_API APigNPCAreaController : public AActor
{
	GENERATED_BODY()
	
public:	
	APigNPCAreaController();
	virtual void BeginPlay() override;

	APigNPCArea* GetClosestArea(const APigNPC* Actor);

protected:
	UPROPERTY(EditInstanceOnly, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "TargetAreas")
	TArray<APigNPCArea*> m_TargetAreas;

};
