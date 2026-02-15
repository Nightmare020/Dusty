// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Dusty/SaveGame/SaveGameInterface.h"
#include "UnlockPoleSkillTrigger.generated.h"

class UBoxComponent;
class UPageWidget;

UCLASS()
class DUSTY_API AUnlockPoleSkillTrigger : public AActor, public ISaveGameInterface
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
		UBoxComponent* Trigger;

	UPROPERTY(EditAnywhere, Category = "Design | Skill")
		bool ExtendiblePoleSkill;

	UPROPERTY(EditAnywhere, Category = "Design | Skill")
		bool LightPoleSkill;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UPageWidget> m_pageToUnlock = nullptr;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	bool m_doOnce = false;

public:
	// Sets default values for this actor's properties
	AUnlockPoleSkillTrigger();

	virtual void BeginPlay() override;
	virtual void LoadActor();

	virtual void OnActorSaved_Implementation();

};
