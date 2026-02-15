// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "BTT_RealTimeMoveTo.generated.h"

/**
 * 
 */
UCLASS()
class DUSTY_API UBTT_RealTimeMoveTo : public UBTTask_MoveTo
{
	GENERATED_BODY()

	EBTNodeResult::Type virtual ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
