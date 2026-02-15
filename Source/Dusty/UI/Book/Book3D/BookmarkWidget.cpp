// Fill out your copyright notice in the Description page of Project Settings.


#include "BookmarkWidget.h"
#include "BookActorTextures.h"
#include "Components/Button.h"

void UBookmarkWidget::B_OpenBookmark()
{
	m_book->OpenBookmarkWithAnimation(m_bookmarkType);
}

void UBookmarkWidget::InitBookmark(ABookActorTextures* _book, LG_Bookmarks _value)
{
	m_book = _book; 
	m_bookmarkType = _value;
	ButtonBookmark->OnReleased.AddDynamic(this, &UBookmarkWidget::B_OpenBookmark);
}
