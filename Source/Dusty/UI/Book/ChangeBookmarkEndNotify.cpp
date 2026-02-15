// Fill out your copyright notice in the Description page of Project Settings.


#include "ChangeBookmarkEndNotify.h"

void UChangeBookmarkEndNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	OnNotified.ExecuteIfBound();
	Super::Notify(MeshComp, Animation, EventReference);
}

