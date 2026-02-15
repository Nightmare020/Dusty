// Fill out your copyright notice in the Description page of Project Settings.


#include "PageWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"

void UPageWidget::PageInit()
{
    m_pageClass = this->GetClass();

    Unlocked_Panel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    Locked_Panel->SetVisibility(ESlateVisibility::Hidden);
    //if (m_isLocked)
    //{
    //    Unlocked_Panel->SetVisibility(ESlateVisibility::Hidden);
    //    Locked_Panel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    //}
    //else
    //{
    //}

    PageNumber_Left->SetText(FText::FromString(""));
    PageNumber_Right->SetText(FText::FromString(""));
}

void UPageWidget::Unlock()
{
    m_isLocked = false; 
    Unlocked_Panel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    Locked_Panel->SetVisibility(ESlateVisibility::Hidden);
}

void UPageWidget::Lock()
{
    m_isLocked = true;
    //Unlocked_Panel->SetVisibility(ESlateVisibility::Hidden);
    //Locked_Panel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

bool UPageWidget::GetIsLocked()
{
    return m_isLocked;
}

void UPageWidget::SetPageNumber(int _value)
{
    m_pageNumber = _value;
    PageNumber_Left->SetText(FText::AsNumber(_value*2+1));
    PageNumber_Right->SetText(FText::AsNumber(_value*2+2));
}

int UPageWidget::GetPageNumber()
{
    return m_pageNumber;
}

TSubclassOf<UPageWidget> UPageWidget::GetPageClass()
{
    return m_pageClass;
}
