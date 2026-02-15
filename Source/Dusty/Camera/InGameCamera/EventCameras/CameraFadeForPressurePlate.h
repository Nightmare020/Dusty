#pragma once

#include "CoreMinimal.h"
#include "CameraFade.h"
#include "CameraFadeForPressurePlate.generated.h"

class APressurePlateController;

UCLASS()
class DUSTY_API ACameraFadeForPressurePlate : public ACameraFade
{
private:
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "PressurePlateController")
	APressurePlateController* m_PressurePlateController = nullptr;

	virtual void Reached() override;
	
};
