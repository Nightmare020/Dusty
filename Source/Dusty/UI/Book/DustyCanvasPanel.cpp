// Fill out your copyright notice in the Description page of Project Settings.


#include "DustyCanvasPanel.h"
#include "Book3D/BookButton.h"

void UDustyCanvasPanel::UpdateVisibility()
{
	if (m_canBeVisible)
	{
		if (m_desiredVisibility)
		{
			SetVisibility(ESlateVisibility::HitTestInvisible);
			ShowChildren();
		}
		else
		{
			SetVisibility(ESlateVisibility::Hidden);
			HideChildren();
		}
	}
	else
	{
		SetVisibility(ESlateVisibility::Hidden);
		HideChildren();
	}
}

void UDustyCanvasPanel::ShowChildren()
{
	TArray<UWidget*> AllChildren = GetAllChildren();
	for (int32 i = 0; i < AllChildren.Num(); i++)
	{
		if (AllChildren[i]->IsA<UDustyCanvasPanel>())
		{
			Cast<UDustyCanvasPanel>(AllChildren[i])->Show();
		}
		else if (AllChildren[i]->IsA<UBookButton>())
		{
			Cast<UBookButton>(AllChildren[i])->SetRealVisibility(true);
		}
		else
		{
			AllChildren[i]->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
	}
}

void UDustyCanvasPanel::HideChildren()
{
	TArray<UWidget*> AllChildren = GetAllChildren();
	for (int32 i = 0; i < AllChildren.Num(); i++)
	{
		if (AllChildren[i]->IsA<UDustyCanvasPanel>())
		{
			Cast<UDustyCanvasPanel>(AllChildren[i])->Hide();
		}
		else if (AllChildren[i]->IsA<UBookButton>())
		{
			Cast<UBookButton>(AllChildren[i])->SetRealVisibility(false);
		}
		else
		{
			AllChildren[i]->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UDustyCanvasPanel::Show()
{
	m_canBeVisible = true;
	UpdateVisibility();
}

void UDustyCanvasPanel::Hide()
{
	m_canBeVisible = false;
	UpdateVisibility();
}

void UDustyCanvasPanel::SetDesiredVisibility(bool _value)
{
	m_desiredVisibility = _value;
	UpdateVisibility();
}
