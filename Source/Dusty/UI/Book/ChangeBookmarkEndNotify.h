// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "ChangeBookmarkEndNotify.generated.h"

DECLARE_DELEGATE(FOnChangeBookmark)

UCLASS()
class DUSTY_API UChangeBookmarkEndNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	FOnChangeBookmark OnNotified;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
