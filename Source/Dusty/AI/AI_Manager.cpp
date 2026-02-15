#include "AI_Manager.h"

void UAI_Manager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	FWorldDelegates::OnPostWorldInitialization.AddUObject(this, &UAI_Manager::OnPostWorldInitialization);
}

void UAI_Manager::OnPostWorldInitialization(UWorld* World, const UWorld::InitializationValues InitValues)
{
	
}

void UAI_Manager::Deinitialize()
{
	Super::Deinitialize();
}
