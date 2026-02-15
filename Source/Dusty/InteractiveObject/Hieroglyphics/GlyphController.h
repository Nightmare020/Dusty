#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Dusty/SaveGame/SaveGameInterface.h"
#include "GlyphController.generated.h"


class AToggleActor;
class AKeyController;
class UPageWidget;

UCLASS()
class DUSTY_API AGlyphController : public AActor, public ISaveGameInterface
{
private:
	GENERATED_BODY()

	UPROPERTY(EditInstanceOnly)
	TArray<AToggleActor*> m_ActorsToActivate;
	UPROPERTY(EditInstanceOnly)
	TMap<AKeyController*, bool> m_KeyControllers;	

	bool m_Solved = false;

	virtual void BeginPlay();
public:

	UPROPERTY(EditInstanceOnly, Category = "Design")
		TSubclassOf<UPageWidget> PageToUnlock = nullptr;

	void SetKeyControllerState(AKeyController* _Controller, bool _State);	

	void LoadActor();

	void OnActorSaved_Implementation();

};
