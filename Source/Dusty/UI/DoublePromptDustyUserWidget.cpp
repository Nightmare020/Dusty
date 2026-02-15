// Fill out your copyright notice in the Description page of Project Settings.


#include "DoublePromptDustyUserWidget.h"
#include "Components/PanelWidget.h"
#include "Components/Image.h"
#include "Dusty/Utils/DustyUtils.h"

void UDoublePromptDustyUserWidget::SetButtonsIds(int FirstButtonId, int SecondButtonId)
{
	m_FirstButtonId = FirstButtonId;
	m_SecondButtonId = SecondButtonId;

	TArray<UWidget*> CanvasChildren = ButtonsBox->GetAllChildren();
	int ButtonIndex = 0;
	for (UWidget* Widget : CanvasChildren)
	{
		UImage* ImageButton = Cast<UImage>(Widget);
		if (ImageButton)
		{
			ImageButton->SetBrushFromTexture(KeyboardButtonImages[ButtonIndex == 0 ? m_FirstButtonId : m_SecondButtonId]);
		}

		ButtonIndex++;
	}
}

void UDoublePromptDustyUserWidget::SwitchInput(bool Gamepad)
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
				ImageButton->SetBrushFromTexture(GamepadButtonImages[ButtonIndex == 0 ? m_FirstButtonId : m_SecondButtonId]);
			}
			else
			{
				ImageButton->SetBrushFromTexture(KeyboardButtonImages[ButtonIndex == 0 ? m_FirstButtonId : m_SecondButtonId]);
			}
		}
		ButtonIndex++;
	}
}

void UDoublePromptDustyUserWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
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