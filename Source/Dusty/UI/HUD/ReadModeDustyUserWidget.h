// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../DustyUserWidget.h"
#include "ReadModeDustyUserWidget.generated.h"

class UTextBlock;

UCLASS()
class DUSTY_API UReadModeDustyUserWidget : public UDustyUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
		UTextBlock* Title;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* Content;

public:
	void SetTexts(FText title, FText content);

};
