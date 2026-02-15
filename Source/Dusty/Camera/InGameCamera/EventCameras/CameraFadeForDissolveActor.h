#pragma once

#include "CoreMinimal.h"
#include "CameraFade.h"
#include "CameraFadeForDissolveActor.generated.h"


class ADissolveActorController;

UCLASS()
class DUSTY_API ACameraFadeForDissolveActor : public ACameraFade
{
private:
	GENERATED_BODY()

public:
	virtual void Reached() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "DissolveController")
	ADissolveActorController* m_DissolveController = nullptr;
	
};