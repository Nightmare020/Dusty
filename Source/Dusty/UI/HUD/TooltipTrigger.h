// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TooltipTrigger.generated.h"

class UBoxComponent;

UCLASS()
class DUSTY_API ATooltipTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATooltipTrigger();

	UPROPERTY(EditAnywhere)
	int TimesBeforeShow = 3;

	int Iterations = 0;

protected:

	UPROPERTY(EditAnywhere)
	UBoxComponent* m_Trigger;
public:	
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

};
