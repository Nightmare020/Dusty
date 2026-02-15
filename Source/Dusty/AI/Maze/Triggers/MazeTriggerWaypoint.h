#pragma once

#include "CoreMinimal.h"
#include "BaseActors/MazeActorTrigger.h"
#include "Engine/TriggerSphere.h"
#include "MazeTriggerWaypoint.generated.h"

class AMazeAIController;
class AMazeNPC;
class ADustyCharacter;
class AHexagonPuzzlePiece;

UCLASS()
class DUSTY_API AMazeTriggerWaypoint : public AMazeActorTrigger
{
private:
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Don't Touch", meta = (AllowPrivateAccess = "true"), DisplayName = "IsEnabled")
	bool m_isEnabled = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Don't Touch", meta = (AllowPrivateAccess = "true"), DisplayName = "Pillar")
	AHexagonPuzzlePiece* m_Pillar = nullptr;

private:
	UPROPERTY(Transient)
	ADustyCharacter* m_player = nullptr;
	
public:
	AMazeTriggerWaypoint();
	virtual void BeginPlay() override;

	virtual void DoBeginOverlap(AActor* OtherActor) override;
	virtual void DoEndOverlap(AActor* OtherActor) override; // only used in AI solver with pillar
	
	void SetIsEnabled(bool _isEnabled);
	bool GetIsEnabled() const;

protected:
	virtual void Toggle(AMazeNPC* _NPC, AMazeAIController* _controller);
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design")
	bool BlocksMobility = false;

	int MazeId = 0; // used to identify the control value for forward and backward
};
