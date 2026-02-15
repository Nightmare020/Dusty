// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../DustyUserWidget.h"
#include "SaveDustyUserWidget.generated.h"

class UImage;

UCLASS()
class DUSTY_API USaveDustyUserWidget : public UDustyUserWidget
{
	GENERATED_BODY()


	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;

		UPROPERTY(EditDefaultsOnly, Category = "HUD")
	TArray<UTexture2D*> SavingImages;


	UPROPERTY(meta = (BindWidget))
		UImage* SavingImage;
	UPROPERTY(meta = (BindWidget))
		UImage* AuxSavingImage;

	UPROPERTY(EditDefaultsOnly, Category = "HUD")
		float TimerBetweenFades = 1;

	float TimeBetweenFades = 0;

	int SavingImageIndex = 0;
	
};
