// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../DustyUserWidget.h"
#include "DiaryNotificationUserWidget.generated.h"


class UTextBlock;
class UImage;

UCLASS()
class DUSTY_API UDiaryNotificationUserWidget : public UDustyUserWidget
{
	GENERATED_BODY()

	virtual void SetId(int id) override;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* NotificationTextBlock;
	UPROPERTY(meta = (BindWidget))
	UImage* NotificationImage;


	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	TArray<FText> NotificationTexts;
	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	TArray<UTexture2D*> NotificationImages;
};
