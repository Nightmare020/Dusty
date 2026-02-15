// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "PolePointLightTrigger.generated.h"

UCLASS()
class DUSTY_API APolePointLightTrigger : public ATriggerBox
{
	GENERATED_BODY()
	
public:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
