// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "Dusty/SaveGame/SaveGameInterface.h"
#include "PageUnlocker.generated.h"

class UPageWidget;

UCLASS()
class DUSTY_API APageUnlocker : public ATriggerBox, public ISaveGameInterface
{
	GENERATED_BODY()

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UPageWidget> m_pageToUnlock = nullptr;
	bool m_doOnce = true;


public:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void BeginPlay() override;


	virtual void LoadActor();

	virtual void OnActorSaved_Implementation();
};
