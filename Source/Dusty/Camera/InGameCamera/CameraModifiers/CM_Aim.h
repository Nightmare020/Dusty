#pragma once

#include "CoreMinimal.h"
#include "DustyCameraModifier.h"
#include "CM_Aim.generated.h"


UCLASS()
class DUSTY_API UCM_Aim : public UDustyCameraModifier
{
private:
	GENERATED_BODY()

public:
	virtual void EnableModifier() override;
};
