// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "StandUpEndNotify.generated.h"

DECLARE_DELEGATE(FOnStandUp)

UCLASS()
class DUSTY_API UStandUpEndNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	FOnStandUp OnNotified;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
