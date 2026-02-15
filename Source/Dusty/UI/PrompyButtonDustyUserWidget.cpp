// Fill out your copyright notice in the Description page of Project Settings.


#include "PrompyButtonDustyUserWidget.h"
#include "Components/PanelWidget.h"
#include "Components/Image.h"
#include "Dusty/Utils/DustyUtils.h"

void UPrompyButtonDustyUserWidget::SetId(int id)
{
	Super::SetId(id);

	TArray<UWidget*> CanvasChildren = ButtonsBox->GetAllChildren();
	for (UWidget* Widget : CanvasChildren)
	{
		UImage* ImageButton = Cast<UImage>(Widget);
		if (ImageButton)
		{
			ImageButton->SetBrushFromTexture(KeyboardButtonImages[id]);
		}
	}
}

void UPrompyButtonDustyUserWidget::SwitchInput(bool Gamepad)
{
	TArray<UWidget*> CanvasChildren = ButtonsBox->GetAllChildren();
	for (UWidget* Widget : CanvasChildren)
	{
		UImage* ImageButton = Cast<UImage>(Widget);
		if (ImageButton)
		{
			if (Gamepad)
			{
				ImageButton->SetBrushFromTexture(GamepadButtonImages[m_id]);
			}
			else
			{
				ImageButton->SetBrushFromTexture(KeyboardButtonImages[m_id]);
			}
		}
	}
}
