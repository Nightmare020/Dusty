#include "ToolActorBase.h"

AToolActorBase::AToolActorBase()
{
#if WITH_EDITORONLY_DATA
	PrimaryActorTick.bCanEverTick = true;
	bAllowTickBeforeBeginPlay = true;
	bIsEditorOnlyActor = true;
#endif
}

bool AToolActorBase::ShouldTickIfViewportsOnly() const
{
	return true;
}