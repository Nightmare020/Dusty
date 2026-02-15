// Fill out your copyright notice in the Description page of Project Settings.


#include "CloseBookEndNotify.h"

void UCloseBookEndNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	OnClosedBook.Broadcast();
	Super::Notify(MeshComp, Animation, EventReference);
}