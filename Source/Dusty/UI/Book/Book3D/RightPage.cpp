//// Fill out your copyright notice in the Description page of Project Settings.
//
//
//#include "RightPage.h"
//#include "BookActorTextures.h"
////#include "Components\Button.h"
//#include "BookButton.h"
////Placeholder for B_Quit() function
//#include "Kismet\KismetSystemLibrary.h"
//
//void URightPage::B_Resume()
//{
//	BookActor->CloseBook();
//}
//
//void URightPage::B_Logbook()
//{
//	BookActor->OpenBook(LG_Bookmarks::LogBook);
//}
//
//void URightPage::B_Glyphs()
//{
//	BookActor->OpenBook(LG_Bookmarks::Glyphs);
//}
//
//void URightPage::B_Settings()
//{
//	BookActor->OpenBook(LG_Bookmarks::Settings);
//}
//
//void URightPage::B_Quit()
//{
//	// PLACEHOLDER
//	//This should call a manager that handles how we quit the game
//	UKismetSystemLibrary::QuitGame(GetWorld(), NULL, EQuitPreference::Quit, true);
//}
//
//void URightPage::B_NextPage()
//{
//	BookActor->LB_NextPage_Start();
//}
//
//void URightPage::Init(ABookActorTextures* _book)
//{
//	Super::Init(_book);
//	
//	m_buttons.Add(Resume_Button);
//	m_buttons.Add(LogBook_Button);
//	m_buttons.Add(Glyphs_Button);
//	m_buttons.Add(Settings_Button);
//	m_buttons.Add(Quit_Button);
//	m_buttons.Add(NextPage_Button);
//
//	m_firstButton = Resume_Button;
//
//	Resume_Button->OnReleased.AddDynamic(this, &URightPage::B_Resume);
//	LogBook_Button->OnReleased.AddDynamic(this, &URightPage::B_Logbook);
//	Glyphs_Button->OnReleased.AddDynamic(this, &URightPage::B_Glyphs);
//	Settings_Button->OnReleased.AddDynamic(this, &URightPage::B_Settings);
//	Quit_Button->OnReleased.AddDynamic(this, &URightPage::B_Quit);
//	NextPage_Button->OnReleased.AddDynamic(this, &URightPage::B_NextPage);
//}
