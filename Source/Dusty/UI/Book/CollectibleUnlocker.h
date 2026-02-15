// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dusty/InteractiveObject/Interactable/Interactable.h"
#include "Dusty/SaveGame/SaveGameInterface.h"
#include "CollectibleUnlocker.generated.h"

class UPageWidget;
class UNiagaraComponent;
class UBillboardComponent;
class UBoxComponent;

UCLASS()
class DUSTY_API ACollectibleUnlocker : public AInteractable, public ISaveGameInterface
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UNiagaraComponent* m_collectibleEffect = nullptr;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UPageWidget> m_pageToUnlock = nullptr;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"))
		int m_collectibleNumber = 1;
	bool m_doOnce = false;

	bool collected = false;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"))
		UBoxComponent* m_boxTrigger = nullptr;

	virtual void LoadActor();

	virtual void OnActorSaved_Implementation();

#if WITH_EDITORONLY_DATA
	// Reference to sprite visualization component
	UPROPERTY()
	UBillboardComponent* SpriteComponent;
#endif

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

public:
	UPROPERTY(EditInstanceOnly)
		bool IsInInitialMap = false;
	void RestartCollectible();
	virtual void BeginPlay() override;
	ACollectibleUnlocker();

	virtual void Interact() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere);
	USoundBase* sound;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere);
	UAudioComponent* m_AudioComponent;
};
