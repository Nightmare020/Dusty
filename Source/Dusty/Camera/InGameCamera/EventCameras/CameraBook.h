#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CameraBook.generated.h"

UCLASS()
class DUSTY_API ACameraBook : public AActor
{
	GENERATED_BODY()
	
public:	
	ACameraBook();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
