#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StateActionsDataAsset.h"
#include "BaseState.h"

#include "ActionsFilter.generated.h"

class ADustyCharacter;
UCLASS()
class DUSTY_API UActionsFilter : public UActorComponent
{
	GENERATED_BODY()

public:	
	UActionsFilter();

	virtual void BeginPlay() override;
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	bool IsStateAvailable(TSubclassOf<UBaseState> _state);
	
	void SetCurrentState(const TSubclassOf<UBaseState> _newState);
	
	UBaseState* GetCurrentBaseState();
	
	TSubclassOf<UBaseState> GetCurrentBaseStateClass();

public:
	UPROPERTY(EditAnywhere)
		UStateActionsDataAsset* StatesDataAsset = nullptr;

	// Unused variable
	// UPROPERTY(EditAnywhere)
	// 	bool DebugLogs;

	UPROPERTY()
		TMap<TSubclassOf<UBaseState>, UBaseState*> m_statesInstancesMap;
	
private:
	UPROPERTY(Transient)
		ADustyCharacter* m_DustyCharacter = nullptr;

	UPROPERTY(Transient)
		UBaseState* m_currentBaseState = nullptr;

	UPROPERTY()
		TSubclassOf<UBaseState> m_currentBaseStateClass;

};

