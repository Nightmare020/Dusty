#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "DustyCameraActor.generated.h"


UCLASS()
class DUSTY_API ADustyCameraActor : public ACameraActor
{
private:
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	void SetFov(float newFov);

private:
	UPROPERTY(Transient)
	class ADustyCharacter* m_Player = nullptr;
	
};
