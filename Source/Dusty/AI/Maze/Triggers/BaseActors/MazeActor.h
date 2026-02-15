#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Dusty/SaveGame/SaveGameInterface.h"
#include "MazeActor.generated.h"

class ADustyCharacter;
class AMazeNPC;

UCLASS()
class DUSTY_API AMazeActor : public AActor, public ISaveGameInterface
{
private:
	GENERATED_BODY()
	
public:	
	AMazeActor();
	
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;

	virtual void LoadActor();

	virtual void OnActorSaved_Implementation();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design|Base")
	AMazeNPC* MazeNPC = nullptr;

	UPROPERTY(Transient)
	ADustyCharacter* Player = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design|Base")
	bool IsActive = true;

};
