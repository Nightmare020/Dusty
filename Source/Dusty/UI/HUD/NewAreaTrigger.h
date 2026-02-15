// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NewAreaTrigger.generated.h"

class UBoxComponent;

UCLASS()
class DUSTY_API ANewAreaTrigger : public AActor
{
	GENERATED_BODY()

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	UPROPERTY(EditAnywhere)
		UBoxComponent* m_Trigger;
	UPROPERTY(EditAnywhere)
		int m_AreaId;
public:	
	// Sets default values for this actor's properties
	ANewAreaTrigger();



};
