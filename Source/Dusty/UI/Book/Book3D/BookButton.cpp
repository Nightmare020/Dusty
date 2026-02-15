// Fill out your copyright notice in the Description page of Project Settings.


#include "BookButton.h"
#include "Components/CanvasPanelSlot.h"
#include "Components\Button.h"
#include "Components\Image.h"
#include "Dusty/Audio/AudioSubsytem.h"
#include "Styling/DefaultStyleCache.h"
#include "Components/VerticalBox.h"
#include "Components/NamedSlot.h"
#include "Components/TextBlock.h"


#include "Dusty/Utils/DustyUtils.h"

void UBookButton::UpdateVisibility()
{
	if (m_canBeVisible)
	{
		if (m_desiredVisibility)
		{
			SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		else
		{
			SetVisibility(ESlateVisibility::Hidden);
		}
	}
	else
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
}

bool UBookButton::IsBookButtonVisible()
{
	return m_canBeVisible && m_desiredVisibility; //IsVisible() && (GetVisibility() == ESlateVisibility::SelfHitTestInvisible || GetVisibility() == ESlateVisibility::Visible);
}

void UBookButton::TryCatchingVisualResources()
{
	if (triedCatchingText == false)
	{
		for (int i = 0; i < ChildSlot->GetChildrenCount(); i++)
		{
			UTextBlock* text = Cast<UTextBlock>(ChildSlot->GetChildAt(i));
			if (text != nullptr)
			{
				PossibleText = text;
				break;
			}
			else
			{
				UImage* image = Cast<UImage>(ChildSlot->GetChildAt(i));
				if (image != nullptr)
				{
					PossibleImage = image;
					break;
				}
			}
		}
		triedCatchingText = true;
	}
}

void UBookButton::Hover()
{
	if(m_buttonSound && IsBookButtonVisible())
	{
		//GetGameInstance()->GetSubsystem<UAudioSubsytem>()->PlaySound2D(m_buttonSound);
		TryCatchingVisualResources();
		background->SetOpacity(1);

		if (!m_isDisabled)
		{

			//background->SetColorAndOpacity(m_clrHighlighted);
			if (PossibleText)
			{
				PossibleText->SetColorAndOpacity(FSlateColor(FColor::White));
			}
			else if (PossibleImage)
			{
				PossibleImage->SetColorAndOpacity(FLinearColor(FColor::White));
			}

		}
	}
}

void UBookButton::Unhover()
{
	TryCatchingVisualResources();
	background->SetOpacity(0);
	if (!m_isDisabled)
	{
		if (PossibleText)
		{
			PossibleText->SetColorAndOpacity(FSlateColor(TextUnhoveredColor));
		}
		else if (PossibleImage)
		{
			PossibleImage->SetColorAndOpacity(FLinearColor(TextUnhoveredColor));
		}
	}
}

void UBookButton::Pressed()
{
	if (!m_isDisabled)
	{
		m_isPressed = true;
		SetPressedColor();

		if (OnPressed.IsBound())
		{
			OnPressed.Broadcast();
		}
	}
}

void UBookButton::Released()
{
	if (!m_isDisabled)// && m_isPressed)
	{
		m_isPressed = false;

		SetHighlightedColor();
		if (OnReleased.IsBound())
		{
			OnReleased.Broadcast();
		}
	}
}

void UBookButton::SetPressedColor()
{
	//background->SetColorAndOpacity(m_clrPressed);
}

void UBookButton::SetHighlightedColor()
{
	//background->SetColorAndOpacity(m_clrHighlighted);
}

void UBookButton::SetDisabled(bool _disabled)
{
	m_isDisabled = _disabled;
	TryCatchingVisualResources();

	if (m_isDisabled)
	{
		//background->SetOpacity(0);
		if (PossibleText)
		{
			PossibleText->SetColorAndOpacity(FSlateColor(TextDisabledColor));
			PossibleText->SetOpacity(0.15f);
		}
		else if (PossibleImage)
		{
			PossibleImage->SetColorAndOpacity(FLinearColor(TextDisabledColor));
			PossibleImage->SetOpacity(0.15f);
		}
	}
	else
	{
		if (PossibleText)
		{
			PossibleText->SetColorAndOpacity(FSlateColor(TextUnhoveredColor));
		}
		else if (PossibleImage)
		{
			PossibleImage->SetColorAndOpacity(FLinearColor(TextUnhoveredColor));
		}
	}
}

void UBookButton::SetDesiredVisibility(bool _value)
{
	m_desiredVisibility = _value;
	UpdateVisibility();
}

void UBookButton::SetRealVisibility(bool _value)
{
	m_canBeVisible = _value;
	UpdateVisibility();
}
