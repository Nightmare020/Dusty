#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Dusty/SaveGame/SaveGameInterface.h"
#include "CameraOverrider.generated.h"

class ADustyCameraManager;
class ADustyCharacter;

UENUM(BlueprintType)
enum class ECameraState : uint8
{
	Entering UMETA(DisplayName = "Entering"),
	Reached  UMETA(DisplayName = "Reached"),
	Exiting  UMETA(DisplayName = "Exiting")
};

UCLASS(abstract)
class DUSTY_API ACameraOverrider : public AActor, public ISaveGameInterface
{
	GENERATED_BODY()
	
public:	
	ACameraOverrider();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void Update(float DeltaTime){}
	
	virtual FRotator GetInputVector();

	virtual bool RefChecker();

	virtual void LoadActor();

	virtual void OnActorSaved_Implementation();
public:
	bool IsOnWait = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", DisplayName = "IsActive")
	bool IsActive = true;

	UPROPERTY(Transient)
	ADustyCharacter* Player = nullptr;

	UPROPERTY(Transient)
	ADustyCameraManager* CameraManager = nullptr;
	
};
