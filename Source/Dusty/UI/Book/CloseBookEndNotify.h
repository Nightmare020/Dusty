// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CloseBookEndNotify.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnCloseBook)

UCLASS()
class DUSTY_API UCloseBookEndNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	FOnCloseBook OnClosedBook;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
