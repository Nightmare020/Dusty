// Fill out your copyright notice in the Description page of Project Settings.


#include "DustyHUD.h"
#include "Dusty/UI/DustyUserWidget.h"
#include "Dusty/UI/DoublePromptDustyUserWidget.h"
#include "Dusty/Utils/DustyUtils.h"
#include "Dusty/UI/HUD/ReadModeDustyUserWidget.h"

void ADustyHUD::ShowHud(EHudCanvas CanvasName, int id)
{
	if (UDustyUserWidget* Widget = m_WidgetsInstancesMap.FindRef(CanvasName))
	{
		if (id != -1)
		{
			Widget->SetId(id);
		}
		Widget->Show();
	}
}

void ADustyHUD::ShowHud(EHudCanvas CanvasName)
{
	if (UDustyUserWidget* Widget = m_WidgetsInstancesMap.FindRef(CanvasName))
	{
		Widget->Show();
	}
}

void ADustyHUD::ShowDoublePromptHud(int FirstButtonId, int SecondButtonId)
{
	if (m_DoublePromptUserWidget)
	{
		m_DoublePromptUserWidget->SetButtonsIds(FirstButtonId, SecondButtonId);
		m_DoublePromptUserWidget->Show();
	}
}

void ADustyHUD::ShowDoublePromptHud()
{
	if (m_DoublePromptUserWidget)
	{
		m_DoublePromptUserWidget->Show();
	}
}

void ADustyHUD::ShowReadMode(FText Title, FText Content)
{
	if (m_ReadModeUserWidget)
	{
		m_ReadModeUserWidget->SetTexts(Title, Content);
		m_ReadModeUserWidget->Show();
	}
}

void ADustyHUD::HideHud(EHudCanvas CanvasName)
{
	if (CanvasName == EHudCanvas::ReadMode)
	{
		m_ReadModeUserWidget->Hide();
	}
	if (UDustyUserWidget* Widget = m_WidgetsInstancesMap.FindRef(CanvasName))
	{
		Widget->Hide();
	}
}

void ADustyHUD::HideHudWithNoFade(EHudCanvas CanvasName)
{
	if (CanvasName == EHudCanvas::ReadMode)
	{
		m_ReadModeUserWidget->HideWithNoFade();
	}
	if (UDustyUserWidget* Widget = m_WidgetsInstancesMap.FindRef(CanvasName))
	{
		Widget->HideWithNoFade();
	}
}


void ADustyHUD::HideDoublePromptHud()
{
	if (m_DoublePromptUserWidget)
	{
		m_DoublePromptUserWidget->Hide();
	}
}

void ADustyHUD::SwapReadModeTexts(FText Title, FText Content)
{
	if (m_ReadModeUserWidget)
	{
		m_ReadModeUserWidget->SetTexts(Title, Content);
	}
}

bool ADustyHUD::IsHudVisible(EHudCanvas CanvasName)
{
	if (CanvasName == EHudCanvas::DoubleButtonPrompt)
	{
		return m_DoublePromptUserWidget->IsVisible();
	}
	if (CanvasName == EHudCanvas::ReadMode)
	{
		return m_ReadModeUserWidget->IsVisible();
	}
	if (UDustyUserWidget* Widget = m_WidgetsInstancesMap.FindRef(CanvasName))
	{
		return Widget->IsVisible();
	}
	return false;
}

void ADustyHUD::BeginPlay()
{
	Super::BeginPlay();

	for (const TPair<EHudCanvas, TSubclassOf<UDustyUserWidget>>& Elem : WidgetsClassesMap)
	{
		EHudCanvas CanvasType = Elem.Key;
		TSubclassOf<UDustyUserWidget> WidgetClass = Elem.Value;

		if (CanvasType == EHudCanvas::DoubleButtonPrompt)
		{
			UDoublePromptDustyUserWidget* WidgetInstance = CreateWidget<UDoublePromptDustyUserWidget>(GetWorld(), WidgetClass);

			WidgetInstance->AddToViewport();
			WidgetInstance->HideWithNoFade();

			m_DoublePromptUserWidget = WidgetInstance;
		}
		else if (CanvasType == EHudCanvas::ReadMode)
		{
			UReadModeDustyUserWidget* WidgetInstance = CreateWidget<UReadModeDustyUserWidget>(GetWorld(), WidgetClass);

			WidgetInstance->AddToViewport(1);
			WidgetInstance->HideWithNoFade();

			m_ReadModeUserWidget = WidgetInstance;

		}
		else if (WidgetClass)
		{
			UDustyUserWidget* WidgetInstance = CreateWidget<UDustyUserWidget>(GetWorld(), WidgetClass);

			WidgetInstance->AddToViewport();
			WidgetInstance->HideWithNoFade();

			m_WidgetsInstancesMap.Add(CanvasType, WidgetInstance);
		}
	}
}
