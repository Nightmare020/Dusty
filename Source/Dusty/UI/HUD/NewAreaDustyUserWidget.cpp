// Fill out your copyright notice in the Description page of Project Settings.


#include "NewAreaDustyUserWidget.h"
#include "Components/TextBlock.h"

void UNewAreaDustyUserWidget::SetId(int id)
{
	Super::SetId(id);

	AreaTextBlock->SetText(AreaTexts[id]);
}
