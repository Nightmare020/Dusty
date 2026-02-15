#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AI_Manager.generated.h"


UCLASS()
class DUSTY_API UAI_Manager : public UGameInstanceSubsystem
{
private:
	GENERATED_BODY()
	
public:
	virtual void Deinitialize() override;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	void OnPostWorldInitialization(UWorld* World, const UWorld::InitializationValues InitValues);
	
};
