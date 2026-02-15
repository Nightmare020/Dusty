// Fill out your copyright notice in the Description page of Project Settings.


#include "DiaryNotificationUserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/BrushComponent.h"


void UDiaryNotificationUserWidget::SetId(int id)
{
	Super::SetId(id);

	NotificationTextBlock->SetText(NotificationTexts[id]);
	NotificationImage->SetBrushFromTexture(NotificationImages[id]);

}
