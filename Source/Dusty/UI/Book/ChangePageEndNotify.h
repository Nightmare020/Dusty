// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "ChangePageEndNotify.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnChangePage)

UCLASS()
class DUSTY_API UChangePageEndNotify : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	FOnChangePage OnNotified;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
