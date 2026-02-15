#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TadpoleController.generated.h"

class ATadpoleSpline;
class ATadpoleNPC;

UCLASS()
class DUSTY_API ATadpoleController : public AActor
{
	GENERATED_BODY()
	
public:	
	ATadpoleController();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	ATadpoleSpline* GetTargetSpline(ATadpoleNPC* _Tadpole, ATadpoleSpline* _Current);
	ATadpoleSpline* GetRandomSpline(ATadpoleSpline* _Current);
	

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "Tadpoles")
	TArray<ATadpoleNPC*> m_Tadpoles;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "Splines")
	TArray<ATadpoleSpline*> m_Splines;

};
