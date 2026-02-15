// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "SittingRock.generated.h"

class UBoxComponent;
class ADustyCharacter;
class UArrowComponent;

UCLASS()
class DUSTY_API ASittingRock : public AInteractable
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* Body = nullptr;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		UArrowComponent* SitPosition = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"))
		UBoxComponent* m_boxTrigger = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UAnimMontage* SitAnimation = nullptr;

public:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Design")
		bool IsNewGameRock = false;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Design")
		bool CanInteract = true;

	ASittingRock();

	void SitDusty();
	void SitDusty(ADustyCharacter* _dusty);
	virtual void Interact() override;
	virtual void OnFinishedAutoMovement() override;
};
