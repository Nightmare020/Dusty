// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "../../Utils/DustyConst.h"
#include "Dusty/SaveGame/SaveGameInterface.h"
#include "GlyphUnlocker.generated.h"

class UPageWidget;

UCLASS()
class DUSTY_API AGlyphUnlocker : public ATriggerBox, public ISaveGameInterface
{
	GENERATED_BODY()

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"))
	EGlyphs GlyphToUnlock = EGlyphs::Light;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UPageWidget> m_pageToUnlock = nullptr;
	bool m_doOnce = true;

	UPROPERTY(EditAnywhere)
	USoundBase* m_sound;

	bool m_soundPlayed = false;

public:
	virtual void BeginPlay() override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void LoadActor();

	virtual void OnActorSaved_Implementation();
};
