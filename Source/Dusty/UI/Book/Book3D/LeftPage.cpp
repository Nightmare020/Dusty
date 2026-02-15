// Fill out your copyright notice in the Description page of Project Settings.


#include "LeftPage.h"
#include "BookActorTextures.h"
//#include "Components\Button.h"
#include "BookButton.h"

void ULeftPage::B_PreviousPage()
{
	BookActor->LB_PreviousPage_Start();
}

void ULeftPage::B_GoPause()
{
	//BookActor->OpenBook(LG_Bookmarks::Pause);
}

//void ULeftPage::Init(ABookActorTextures* _book)
//{
//	Super::Init(_book);
//
//	m_buttons.Add(PreviousPage_Button);
//	m_buttons.Add(GoPause_Button);
//
//	m_firstButton = GoPause_Button;
//	
//	PreviousPage_Button->OnReleased.AddDynamic(this, &ULeftPage::B_PreviousPage);
//	GoPause_Button->OnReleased.AddDynamic(this, &ULeftPage::B_GoPause);
//}
