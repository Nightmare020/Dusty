// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShowFinalQuestionTrigger.generated.h"

class UBoxComponent;
class ADustyCharacter;

UCLASS()
class DUSTY_API AShowFinalQuestionTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShowFinalQuestionTrigger();
private:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		UBoxComponent* m_Trigger;

	ADustyCharacter* Miau;

};
