// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FixedFailStateTrigger.generated.h"

class UBoxComponent;

UCLASS()
class DUSTY_API AFixedFailStateTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFixedFailStateTrigger();
private:
	virtual void BeginPlay() override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

	UPROPERTY(EditAnywhere)
		UBoxComponent* Trigger;
	UPROPERTY(EditAnywhere, meta = (MakeEditWidget = true))
		FVector m_DustyRespawnPosition = FVector::ZeroVector;
};
