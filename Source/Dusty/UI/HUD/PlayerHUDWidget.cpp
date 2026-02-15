// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUDWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "TimerManager.h"
#include "../../Utils/DustyUtils.h"

void UPlayerHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPlayerHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	UpdateFade(InDeltaTime);
}



bool UPlayerHUDWidget::IsSkipCinematicCanvasVisible()
{
	return SkipCinematicCanvas->IsVisible();
}

void UPlayerHUDWidget::SkipCinematicCanvasVisible()
{
	SkipCinematicCanvas->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UPlayerHUDWidget::SkipCinematicCanvasHidden()
{
	SkipCinematicCanvas->SetVisibility(ESlateVisibility::Hidden);
}



void UPlayerHUDWidget::FadeInCanvas(UCanvasPanel* CanvasPanel, float Duration)
{
	if (!CanvasPanel) return;

	m_IsFadingIn = true;
	m_IsFadingOut = false;
	m_CurrentFadeTime = 0.f;
	m_FadeDuration = Duration;
	m_TargetCanvasPanel = CanvasPanel;

	CanvasPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	CanvasPanel->SetRenderOpacity(0.f);
}

void UPlayerHUDWidget::FadeOutCanvas(UCanvasPanel* CanvasPanel, float Duration)
{
	if (!CanvasPanel) return;

	m_IsFadingIn = false;
	m_IsFadingOut = true;
	m_CurrentFadeTime = 0.f;
	m_FadeDuration = Duration;
	m_TargetCanvasPanel = CanvasPanel;

	CanvasPanel->SetRenderOpacity(1.0f);
}

void UPlayerHUDWidget::UpdateFade(float DeltaTime)
{
	if (m_IsFadingIn || m_IsFadingOut)
	{
		if (m_TargetCanvasPanel)
		{
			m_CurrentFadeTime += DeltaTime;
			float Alpha = FMath::Clamp(m_CurrentFadeTime / m_FadeDuration, 0.f, 1.f);
			float Opacity = m_IsFadingIn ? Alpha : 1.f - Alpha;
			m_TargetCanvasPanel->SetRenderOpacity(Opacity);

			if (m_CurrentFadeTime >= m_FadeDuration)
			{
				if (m_IsFadingOut)
				{
					m_TargetCanvasPanel->SetVisibility(ESlateVisibility::Hidden);
				}
				m_IsFadingIn = false;
				m_IsFadingOut = false;
				m_TargetCanvasPanel = nullptr;
			}
		}
	}
}