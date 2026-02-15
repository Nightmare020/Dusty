#pragma once

#include "CoreMinimal.h"
#include "PressurePlateController.h"
#include "PressurePlateDissolveController.generated.h"

class ADissolveActor;

UCLASS()
class DUSTY_API APressurePlateDissolveController : public APressurePlateController
{
private:
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "DissolveTime")
	float m_DissolveTime = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "IsLighted")
	bool m_IsLighted = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "DissolveEntities")
	TArray<ADissolveActor*> DissolveEntities;
	
};
