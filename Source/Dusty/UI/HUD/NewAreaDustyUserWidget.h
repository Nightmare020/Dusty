// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../DustyUserWidget.h"
#include "NewAreaDustyUserWidget.generated.h"


class UTextBlock;

UCLASS()
class DUSTY_API UNewAreaDustyUserWidget : public UDustyUserWidget
{
	GENERATED_BODY()


		virtual void SetId(int id) override;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* AreaTextBlock;


	UPROPERTY(EditDefaultsOnly, Category = "HUD")
		TArray<FText> AreaTexts;
};
