// Fill out your copyright notice in the Description page of Project Settings.


#include "HasButtonDustyUserWidget.h"
#include "Components/PanelWidget.h"
#include "Components/Image.h"
#include "Dusty/Utils/DustyUtils.h"

void UHasButtonDustyUserWidget::SwitchInput(bool Gamepad)
{
	TArray<UWidget*> CanvasChildren = ButtonsBox->GetAllChildren();
	int ButtonIndex = 0;
	for (UWidget* Widget : CanvasChildren)
	{
		UImage* ImageButton = Cast<UImage>(Widget);
		if (ImageButton)
		{
			if (Gamepad)
			{
				ImageButton->SetBrushFromTexture(GamepadButtonImages[ButtonIndex++]);
			}
			else
			{
				ImageButton->SetBrushFromTexture(KeyboardButtonImages[ButtonIndex++]);
			}
		}
	}
}

void UHasButtonDustyUserWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);
	ECommonInputType InputType = DustyUtils::GetCurrentInputType();

	
	// Check if the input is Keyboard/Mouse or Gamepad
	if (InputType == ECommonInputType::MouseAndKeyboard)
	{
		SwitchInput(false);
			
	}
	else if (InputType == ECommonInputType::Gamepad)
	{
		SwitchInput(true);
	}
	
}
