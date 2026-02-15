// Fill out your copyright notice in the Description page of Project Settings.


#include "DustyUserWidget.h"
#include "Dusty/Utils/DustyUtils.h"

void UDustyUserWidget::Show()
{
	m_IsFadingIn = true;
	m_IsFadingOut = false;
	m_CurrentFadeTime = 0.f;
	SetVisibility(ESlateVisibility::Visible); 

	SetRenderOpacity(0);
	if (m_AutoExit)
	{
		GetWorld()->GetTimerManager().SetTimer(m_ShowTimeHandle, this, &UDustyUserWidget::Hide, m_ShowTime, false);
	}

	EnterAnimation();

}

void UDustyUserWidget::Hide()
{
	m_IsFadingIn = false;
	m_IsFadingOut = true;
	m_CurrentFadeTime = 0.f;

	ExitAnimation();
}

void UDustyUserWidget::HideWithNoFade()
{
	SetVisibility(ESlateVisibility::Hidden);
}

void UDustyUserWidget::SetId(int id)
{
	m_id = id;
}

void UDustyUserWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);
	UpdateFade(DeltaTime);
}

void UDustyUserWidget::UpdateFade(float DeltaTime)
{	
	if (m_IsFadingIn)
	{
		m_CurrentFadeTime += DeltaTime;
		float Alpha = FMath::Clamp(m_CurrentFadeTime / m_EnterFadeDuration, 0.f, 1.f);
		float Opacity = m_IsFadingIn ? Alpha : 1.f - Alpha;
		SetRenderOpacity(Opacity);

		if (m_CurrentFadeTime >= m_EnterFadeDuration)
		{
			if (m_IsFadingOut)
			{
				SetVisibility(ESlateVisibility::Hidden);
			}
			m_IsFadingIn = false;
			m_IsFadingOut = false;
		}		
	}
	else if (m_IsFadingOut)
	{
		m_CurrentFadeTime += DeltaTime;
		float Alpha = FMath::Clamp(m_CurrentFadeTime / m_ExitFadeDuration, 0.f, 1.f);
		float Opacity = m_IsFadingIn ? Alpha : 1.f - Alpha;
		SetRenderOpacity(Opacity);

		if (m_CurrentFadeTime >= m_ExitFadeDuration)
		{
			if (m_IsFadingOut)
			{
				SetVisibility(ESlateVisibility::Hidden);
			}
			m_IsFadingIn = false;
			m_IsFadingOut = false;
		}
	}
}
