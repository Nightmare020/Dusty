// Fill out your copyright notice in the Description page of Project Settings.


#include "StandUpEndNotify.h"

void UStandUpEndNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	OnNotified.ExecuteIfBound();
	Super::Notify(MeshComp, Animation, EventReference);
}
