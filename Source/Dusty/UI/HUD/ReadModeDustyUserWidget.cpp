// Fill out your copyright notice in the Description page of Project Settings.


#include "ReadModeDustyUserWidget.h"
#include "Components/TextBlock.h"

void UReadModeDustyUserWidget::SetTexts(FText title, FText content)
{
	Title->SetText(title);
	Content->SetText(content);
}
