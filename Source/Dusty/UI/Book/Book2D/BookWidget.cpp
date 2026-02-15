#include "BookWidget.h"	
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "PageWidget.h"
#include "Components\Button.h"
#include "Components/WidgetSwitcher.h"
#include "Dusty/Utils/DustyUtils.h"


//Placeholder for B_Quit() function
#include "Kismet\KismetSystemLibrary.h"

void UBookWidget::NativeConstruct()
{
	Super::NativeConstruct();
	LB_CreatePages();

	//Buttons
	Resume_Button->OnClicked.AddDynamic(this, &UBookWidget::B_Resume);
	LogBook_Button->OnClicked.AddDynamic(this, &UBookWidget::B_Logbook);
	Glyphs_Button->OnClicked.AddDynamic(this, &UBookWidget::B_Glyphs);
	Quit_Button->OnClicked.AddDynamic(this, &UBookWidget::B_Quit);
	NextPage_Button->OnClicked.AddDynamic(this, &UBookWidget::B_NextPage);
	PreviousPage_Button->OnClicked.AddDynamic(this, &UBookWidget::B_PreviousPage);
	ReturnPause_Button->OnClicked.AddDynamic(this, &UBookWidget::B_GoPause);
	ShowNames_Button->OnClicked.AddDynamic(this, &UBookWidget::B_ShowGlyphsNames);
}

void UBookWidget::OpenBook(LG_Bookmarks _bookmark)
{
	m_isOpen = true;

	APlayerController* PlayerController = GetOwningPlayer<APlayerController>();
	PlayerController->SetInputMode(FInputModeGameAndUI());
	PlayerController->bShowMouseCursor = true;
	PlayerController->SetPause(true);

	if (!Book->IsVisible())
	{
		Book->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	switch (_bookmark)
	{
	//case LG_Bookmarks::Pause:
	//	PauseMenu->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	//	Pages->SetVisibility(ESlateVisibility::Hidden);
	//	GlyphsMenu->SetVisibility(ESlateVisibility::Hidden);
	//	ReturnPause_Button->SetVisibility(ESlateVisibility::Hidden);
	//	Resume_Button->SetFocus();
	//	break;

	case LG_Bookmarks::LogBook:
		m_currentPage = 0;
		RightPage->SetActiveWidgetIndex(0);
		LeftPage->SetActiveWidgetIndex(0);
		Pages->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		PauseMenu->SetVisibility(ESlateVisibility::Hidden);
		GlyphsMenu->SetVisibility(ESlateVisibility::Hidden);
		ReturnPause_Button->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		NextPage_Button->SetFocus();
		PreviousPage_Button->SetIsEnabled(false);
		NextPage_Button->SetIsEnabled(true);
		break;

	case LG_Bookmarks::Settings:
		//Se abren unas settings wapardas
		Pages->SetVisibility(ESlateVisibility::Hidden);
		PauseMenu->SetVisibility(ESlateVisibility::Hidden);
		GlyphsMenu->SetVisibility(ESlateVisibility::Hidden);
		ReturnPause_Button->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		break;

	case LG_Bookmarks::Glyphs:
		GlyphsMenu->SetVisibility(ESlateVisibility::Visible);
		GlyphsNames->SetVisibility(ESlateVisibility::Hidden);
		Pages->SetVisibility(ESlateVisibility::Hidden);
		PauseMenu->SetVisibility(ESlateVisibility::Hidden);
		ShowNames_Button->SetFocus();
		ReturnPause_Button->SetVisibility(ESlateVisibility::Visible);
		break;

	default:
		break;
	}

}

void UBookWidget::CloseBook()
{
	m_isOpen = false;
	APlayerController* PlayerController = GetOwningPlayer<APlayerController>();
	PlayerController->SetPause(false);
	PlayerController->SetInputMode(FInputModeGameOnly());
	PlayerController->bShowMouseCursor = false;

	Book->SetVisibility(ESlateVisibility::Hidden);
}

void UBookWidget::ToggleBook()
{
	if (m_isOpen)
	{
		CloseBook();
	}
	else
	{
		//OpenBook(LG_Bookmarks::Pause);
	}
}

#pragma region LogBook Functions
void UBookWidget::LB_NextPage()
{
	int nextpage = m_currentPage + 2;
	if (nextpage < m_maxPageNumberUnlocked)
	{
		m_currentPage += 2;

		RightPage->SetActiveWidgetIndex(m_currentPage / 2);
		LeftPage->SetActiveWidgetIndex(m_currentPage / 2);

		UPageWidget* right = Cast<UPageWidget>(RightPage->GetActiveWidget());
		UPageWidget* left = Cast<UPageWidget>(LeftPage->GetActiveWidget());

		//Disable the nextPage button in case there is no next page
		nextpage = m_currentPage + 2;
		if (nextpage >= m_maxPageNumberUnlocked)
		{
			/*NextPage_Button->SetIsEnabled(false);*/
		}

		//Enable Previous page
		if (!PreviousPage_Button->GetIsEnabled())
		{
			PreviousPage_Button->SetIsEnabled(true);

		}
	}
}

void UBookWidget::LB_PreviousPage()
{
	int nextpage = m_currentPage - 2;
	if (nextpage >= 0)
	{
		m_currentPage -= 2;

		RightPage->SetActiveWidgetIndex(m_currentPage / 2);
		LeftPage->SetActiveWidgetIndex(m_currentPage / 2);

		UPageWidget* right = Cast<UPageWidget>(RightPage->GetActiveWidget());
		UPageWidget* left = Cast<UPageWidget>(LeftPage->GetActiveWidget());

		//Disable the previusPage button in case there is no next page
		nextpage = m_currentPage - 2;
		if (nextpage < 0)
		{
			//PreviusPage_Button->SetIsEnabled(false);
		}

		//Enable Previus page
		if (!NextPage_Button->GetIsEnabled())
		{
			NextPage_Button->SetIsEnabled(true);
		}
	}
}

void UBookWidget::LB_CreatePages()
{
	m_maxPageNumberUnlocked = 0;

	for (int i = 0; i < PagesClasses.Num(); ++i)
	{
		UPageWidget* pageToAdd = CreateWidget<UPageWidget>(GetWorld(), PagesClasses[i]);
		pageToAdd->PageInit();

		// There is no page 0
		pageToAdd->SetPageNumber(i+1);
		if (pageToAdd->GetPageNumber() % 2 == 0)
		{
			RightPage->AddChild(pageToAdd);
		}
		else
		{
			LeftPage->AddChild(pageToAdd);
		}
		
		if (!pageToAdd->GetIsLocked())
		{
			//++m_totalUnlockedPages;
			if (pageToAdd->GetPageNumber() > m_maxPageNumberUnlocked)
			{
				m_maxPageNumberUnlocked = pageToAdd->GetPageNumber();
			}
		}
		else
		{
			m_pagesLockedList.Add(pageToAdd);
		}
	}
}

void UBookWidget::LB_UnlockPage(int _pageNumber)
{
	for (int i = 0; i < m_pagesLockedList.Num(); ++i)
	{
		if (m_pagesLockedList[i]->GetPageNumber() == _pageNumber)
		{
			//++m_totalUnlockedPages;
			if (_pageNumber > m_maxPageNumberUnlocked)
			{
				m_maxPageNumberUnlocked = _pageNumber;

				//DustyUtils::PrintMessage("New MaxPage: %i", m_maxPageNumberUnlocked, FColor::Green, 30);
			}
			m_pagesLockedList[i]->Unlock();
			return;
		}
	}
}

void UBookWidget::LB_UnlockPage(TSubclassOf<UPageWidget> _pageToUnlock)
{
	for (int i = 0; i < m_pagesLockedList.Num(); ++i)
	{
		if (m_pagesLockedList[i]->GetPageClass() == _pageToUnlock)
		{
			//++m_totalUnlockedPages;
			if (m_pagesLockedList[i]->GetPageNumber() > m_maxPageNumberUnlocked)
			{
				m_maxPageNumberUnlocked = m_pagesLockedList[i]->GetPageNumber();

				//DustyUtils::PrintMessage("New MaxPage: %i", m_maxPageNumberUnlocked, FColor::Green, 30);
			}

			m_pagesLockedList[i]->Unlock();
			return;
		}
	}
}

#pragma region ButtonFunctions
void UBookWidget::B_Resume()
{
	CloseBook();
}

void UBookWidget::B_Logbook()
{
	OpenBook(LG_Bookmarks::LogBook);
}

void UBookWidget::B_Glyphs()
{
	OpenBook(LG_Bookmarks::Glyphs);
}

//void UBookWidget::B_Settings()
//{
//	OpenBook(LG_Bookmarks::Settings);
//}

void UBookWidget::B_Quit()
{
	// PLACEHOLDER
	//This should call a manager that handles how we quit the game
	UKismetSystemLibrary::QuitGame(GetWorld(), NULL, EQuitPreference::Quit, true);
}

void UBookWidget::B_NextPage()
{
	LB_NextPage();
}

void UBookWidget::B_PreviousPage()
{
	LB_PreviousPage();
}

void UBookWidget::B_GoPause()
{
	//OpenBook(LG_Bookmarks::Pause);
}

void UBookWidget::B_ShowGlyphsNames()
{
	if (GlyphsNames->IsVisible())
	{
		GlyphsNames->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		GlyphsNames->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

#pragma endregion



void UBookWidget::ShowGlyphLight()
{
	LightImg->SetOpacity(1);
	LightGlyph->SetOpacity(1);
	LightGlyph->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UBookWidget::ShowGlyphSky()
{
	SkyImg->SetOpacity(1);
	SkyGlyph->SetOpacity(1);
	SkyGlyph->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}
