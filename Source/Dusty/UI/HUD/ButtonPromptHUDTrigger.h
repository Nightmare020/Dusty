// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ButtonPromptHUDTrigger.generated.h"

class UBoxComponent;

UCLASS()
class DUSTY_API AButtonPromptHUDTrigger : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		int m_FirstButtonId = 0;
	UPROPERTY(EditAnywhere)
		int m_SecondButtonId = 0;
	UPROPERTY(EditAnywhere)
		bool m_IsDualPrompt = false;
	UPROPERTY(EditAnywhere)
		bool m_NeedsExtendSkill = false;
	UPROPERTY(EditAnywhere)
		bool m_NeedsLightSkill = false;

	UPROPERTY(EditAnywhere)
		UBoxComponent* m_Trigger;
public:	
	// Sets default values for this actor's properties
	AButtonPromptHUDTrigger();

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
	

};
