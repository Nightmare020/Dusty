#pragma once

#include "CoreMinimal.h"
#include "DissolveActorController.h"
#include "DissolveActorAIController.generated.h"


class AMazeNPC;

UCLASS()
class DUSTY_API ADissolveActorAIController : public ADissolveActorController
{
private:
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "NPC")
	AMazeNPC* m_NPC = nullptr;
	
public:
	virtual void Activate(bool _bActivation) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void LoadActor() override;

	virtual void OnActorSaved_Implementation() override;
private:
	bool HasBeenUsed = false;
	float m_NPCActivationTimer = 0.f;
	
};
