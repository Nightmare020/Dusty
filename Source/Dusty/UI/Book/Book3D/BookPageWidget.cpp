// Fill out your copyright notice in the Description page of Project Settings.


#include "BookPageWidget.h"
#include "../Book2D/PageWidget.h"
#include "../DustyCanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "BookActorTextures.h"
#include "Components/WidgetSwitcher.h"
#include "Components/TextBlock.h"
#include "Components/Slider.h"
#include "Dusty/Utils/DustyUtils.h"
#include "Dusty/Cinematics/CinematicsSubsystem.h"
#include "Dusty/Settings/SettingsSubsystem.h"
#include "Dusty/UI/MainMenu/SettingsWidget.h"
#include "Dusty/UI/HUD/PlayerHUDWidget.h"
#include "Dusty/Character/DustyCharacter.h"
#include "Dusty/DustyGameInstance.h"
#include "Dusty/SaveGame/DustySaveGame.h"
#include "Dusty/UI/MainMenu/MainMenuWidget.h"
#include "Kismet/GameplayStatics.h"

#include "BookButton.h"
#include "Dusty/Character/CharacterStates/ActionsFilter.h"

//Placeholder for B_Quit() function
#include "Kismet\KismetSystemLibrary.h"

//Test
#include "Components/Image.h"
#include "Runtime/UMG/Public/Blueprint/WidgetLayoutLibrary.h"

void UBookPageWidget::ResetAllButtons()
{
	for (int i = 0; i < m_buttons.Num(); i++)
	{
		m_buttons[i]->Unhover();
	}
}

void UBookPageWidget::SetCurrentButton(UBookButton* _button)
{
	if (m_currentButton)
	{
		m_currentButton->Unhover();
	}

	m_currentButton = _button;

	if (m_currentButton)
	{
		DustyUtils::PrintMessage("SET CURRENT BUTTON: " + m_currentButton->GetName(), FColor::Red, 10);
		m_currentButton->Hover();
	}
}

void UBookPageWidget::OpenBookmark(LG_Bookmarks _bookmark)
{
	ECommonInputType InputType = DustyUtils::GetCurrentInputType();

	// Get the player character reference
	ADustyCharacter* dusty = DustyUtils::GetDustyCharacter();

	ResetAllButtons();
	switch (_bookmark)
	{
	case LG_Bookmarks::Collectibles :
		collectiblesMenu->Show();
		collectiblesSwitcher->SetActiveWidgetIndex(0);
		AllCollectiblesMenu->SetDesiredVisibility(true);
		BackToCollectibles_Button->SetDesiredVisibility(false);
		pagesMenu->Hide();
		settingsMenu->Hide();
		glyphsMenu->Hide();
		FinalQuestionCanvas->Hide();
		if (InputType == ECommonInputType::Gamepad)
		{
			SetCurrentButton(collectiblesMenu->FirstButton);
		}
		break;

	case LG_Bookmarks::LogBook :
		pagesMenu->Show();
		pagesSwitcher->SetActiveWidgetIndex(BookActor->GetCurrentPage());
		collectiblesMenu->Hide();
		settingsMenu->Hide();
		glyphsMenu->Hide();
		FinalQuestionCanvas->Hide();
		if (InputType == ECommonInputType::Gamepad)
		{
			SetCurrentButton(pagesMenu->FirstButton);
		}

		break;

	case LG_Bookmarks::Settings :
		/*BookActor->CloseBook();
		dusty->GetSettingsMenuWidget().OpenSettingsMenu();
		dusty->GetSettingsMenuWidget().SetWasInBookMenu(true);
		dusty->GetHUDWidget()->BookTutorialCanvasHidden();*/
		TryToChangeSettings();
		settingsMenu->Show();
		collectiblesMenu->Hide();
		pagesMenu->Hide();
		glyphsMenu->Hide();
		FinalQuestionCanvas->Hide();
		if (InputType == ECommonInputType::Gamepad)
		{
			SetCurrentButton(settingsMenu->FirstButton);
		}
		break;

	case LG_Bookmarks::Glyphs :
		glyphsMenu->Show();
		settingsMenu->Hide();
		collectiblesMenu->Hide();
		pagesMenu->Hide();
		FinalQuestionCanvas->Hide();
		if (InputType == ECommonInputType::Gamepad)
		{
			SetCurrentButton(glyphsMenu->FirstButton);
		}
		break;

	case LG_Bookmarks::FinalQuestion :
		glyphsMenu->Hide();
		settingsMenu->Hide();
		collectiblesMenu->Hide();
		pagesMenu->Hide();
		FinalQuestionCanvas->Show();
		if (InputType == ECommonInputType::Gamepad)
		{
			SetCurrentButton(FinalQuestionCanvas->FirstButton);
		}
		break;

	default:
		break;
	}

	QuitMenu->Hide();
}

void UBookPageWidget::Init(ABookActorTextures* _book, UBookPageWidget* _aux)
{
	BookActor = _book;
	AuxiliarWidget = _aux;
	LB_CreatePages();
	LB_CreateCollectibles();

	InitializeButtons();

	TArray<UWidget*> AllWidgets = AllCollectiblesMenu->GetAllChildren();
	for (UWidget* Widget : AllWidgets)
	{
		UBookButton* Button = Cast<UBookButton>(Widget);
		if (Button)
		{
			m_buttons.Add(Button);
		}
	}	

	InitializeElementsVisibility();
}

void UBookPageWidget::InitializeButtons()
{
	m_buttons.Empty();
	m_buttons.Add(BackToCollectibles_Button);
	m_buttons.Add(Quit_Button);
	m_buttons.Add(Quit_Button_Yes);
	m_buttons.Add(Quit_Button_No);
	m_buttons.Add(FinalQuestionButtonYes);
	m_buttons.Add(FinalQuestionButtonNo);

	m_buttons.Add(DisplayModeLeft_Button);
	m_buttons.Add(DisplayModeRight_Button);
	m_buttons.Add(ResolutionLeft_Button);
	m_buttons.Add(ResolutionRight_Button);
	m_buttons.Add(GeneralLeft_Button);
	m_buttons.Add(GeneralRight_Button);
	m_buttons.Add(MusicLeft_Button);
	m_buttons.Add(MusicRight_Button);
	m_buttons.Add(EffectLeft_Button);
	m_buttons.Add(EffectRight_Button);
	m_buttons.Add(CameraSensitivityLeft_Button);
	m_buttons.Add(CameraSensitivityRight_Button);
	m_buttons.Add(ApplyChanges_Button);
	m_buttons.Add(AimCameraSensitivityLeft_Button);
	m_buttons.Add(AimCameraSensitivityRight_Button);

	m_buttons.Add(collectible1_Button);
	m_buttons.Add(collectible2_Button);
	m_buttons.Add(collectible3_Button);
	m_buttons.Add(collectible4_Button);
	m_buttons.Add(collectible5_Button);
	m_buttons.Add(collectible6_Button);
	m_buttons.Add(collectible7_Button);
	m_buttons.Add(collectible9_Button);
	m_buttons.Add(collectible10_Button);
	m_buttons.Add(collectible11_Button);
	m_buttons.Add(collectible12_Button);
	m_buttons.Add(collectible13_Button);
	m_buttons.Add(collectible14_Button);
	m_buttons.Add(collectible15_Button);

	BackToCollectibles_Button->OnReleased.AddDynamic(this, &UBookPageWidget::B_BackToCollectibles);
	Quit_Button->OnReleased.AddDynamic(this, &UBookPageWidget::B_QuitGame);
	Quit_Button_Yes->OnReleased.AddDynamic(this, &UBookPageWidget::B_QuitGame_Yes);
	Quit_Button_No->OnReleased.AddDynamic(this, &UBookPageWidget::B_QuitGame_No);
	FinalQuestionButtonYes->OnReleased.AddDynamic(this, &UBookPageWidget::B_ShowLastCinematicQuest);
	FinalQuestionButtonNo->OnReleased.AddDynamic(this, &UBookPageWidget::B_ShowLastCinematicNoQuest);
	DisplayModeLeft_Button->OnReleased.AddDynamic(this, &UBookPageWidget::B_DisplayModeLeft);
	DisplayModeRight_Button->OnReleased.AddDynamic(this, &UBookPageWidget::B_DisplayModeRight);
	ResolutionLeft_Button->OnReleased.AddDynamic(this, &UBookPageWidget::B_ResolutionLeft);
	ResolutionRight_Button->OnReleased.AddDynamic(this, &UBookPageWidget::B_ResolutionRight);
	ApplyChanges_Button->OnReleased.AddDynamic(this, &UBookPageWidget::B_ApplyChanges);

	GeneralLeft_Button->OnPressed.AddDynamic(this, &UBookPageWidget::B_GeneralVolumeLeftPressed);
	GeneralLeft_Button->OnReleased.AddDynamic(this, &UBookPageWidget::B_GeneralVolumeLeftReleased);
	GeneralRight_Button->OnPressed.AddDynamic(this, &UBookPageWidget::B_GeneralVolumeRightPressed);
	GeneralRight_Button->OnReleased.AddDynamic(this, &UBookPageWidget::B_GeneralVolumeRightReleased);
	MusicLeft_Button->OnPressed.AddDynamic(this, &UBookPageWidget::B_MusicVolumeLeftPressed);
	MusicLeft_Button->OnReleased.AddDynamic(this, &UBookPageWidget::B_MusicVolumeLeftReleased);
	MusicRight_Button->OnPressed.AddDynamic(this, &UBookPageWidget::B_MusicVolumeRightPressed);
	MusicRight_Button->OnReleased.AddDynamic(this, &UBookPageWidget::B_MusicVolumeRightReleased);
	EffectLeft_Button->OnPressed.AddDynamic(this, &UBookPageWidget::B_EffectsVolumeLeftPressed);
	EffectLeft_Button->OnReleased.AddDynamic(this, &UBookPageWidget::B_EffectsVolumeLeftReleased);
	EffectRight_Button->OnPressed.AddDynamic(this, &UBookPageWidget::B_EffectsVolumeRightPressed);
	EffectRight_Button->OnReleased.AddDynamic(this, &UBookPageWidget::B_EffectsVolumeRightReleased);
	CameraSensitivityLeft_Button->OnPressed.AddDynamic(this, &UBookPageWidget::B_CameraSensitivityLeftPressed);
	CameraSensitivityLeft_Button->OnReleased.AddDynamic(this, &UBookPageWidget::B_CameraSensitivityLeftReleased);
	CameraSensitivityRight_Button->OnPressed.AddDynamic(this, &UBookPageWidget::B_CameraSensitivityRightPressed);
	CameraSensitivityRight_Button->OnReleased.AddDynamic(this, &UBookPageWidget::B_CameraSensitivityRightReleased);
	AimCameraSensitivityLeft_Button->OnPressed.AddDynamic(this, &UBookPageWidget::B_AimCameraSensitivityLeftPressed);
	AimCameraSensitivityLeft_Button->OnReleased.AddDynamic(this, &UBookPageWidget::B_AimCameraSensitivityLeftReleased);
	AimCameraSensitivityRight_Button->OnPressed.AddDynamic(this, &UBookPageWidget::B_AimCameraSensitivityRightPressed);
	AimCameraSensitivityRight_Button->OnReleased.AddDynamic(this, &UBookPageWidget::B_AimCameraSensitivityRightReleased);
}

void UBookPageWidget::ToggleReadMode()
{
	if (DustyUtils::IsHudVisible(GetWorld(), EHudCanvas::ReadMode))
	{
		DustyUtils::HideReadMode(GetWorld());
		return;
	}


	UPageWidget* PageWidget = nullptr;
	if (pagesMenu->IsVisible())
	{
		PageWidget = Cast<UPageWidget>(pagesSwitcher->GetActiveWidget());	
	}
	else if (collectiblesMenu->IsVisible())
	{
		PageWidget = Cast<UPageWidget>(collectiblesSwitcher->GetActiveWidget());
	}
	else if (FinalQuestionCanvas->IsVisible())
	{
		FText Title;
		FText Content;

		FString ContentTexts;
		TArray<UWidget*> CanvasChildren = FinalQuestionCanvas->GetAllChildren();
		int TextIndex = 0;
		for (UWidget* Widget : CanvasChildren)
		{
			UTextBlock* TextBlock = Cast<UTextBlock>(Widget);
			if (TextBlock)
			{
				if (TextIndex == 0)
				{
					Title = TextBlock->GetText();
				}
				else
				{
					ContentTexts += "\n\n" + TextBlock->GetText().ToString();
				}
			}
			TextIndex++;
		}


		Content = FText::FromString(ContentTexts);

		DustyUtils::ShowReadMode(GetWorld(), Title, Content);
	}


	if (PageWidget)
	{

		FText Title;
		FText Content;

		FString ContentTexts;
		TArray<UWidget*> CanvasChildren = PageWidget->Unlocked_Panel->GetAllChildren();
		int TextIndex = 0;
		for (UWidget* Widget : CanvasChildren)
		{
			UTextBlock* TextBlock = Cast<UTextBlock>(Widget);
			if (TextBlock)
			{
				if (TextIndex == 0)
				{
					Title = TextBlock->GetText();
				}
				else
				{
					ContentTexts += "\n\n" + TextBlock->GetText().ToString();
				}
			}
			TextIndex++;
		}


		Content = FText::FromString(ContentTexts);

		DustyUtils::ShowReadMode(GetWorld(), Title, Content);
	}
}

void UBookPageWidget::InitializeElementsVisibility()
{
	Glyph_House->SetDesiredVisibility(false);
	Glyph_Person->SetDesiredVisibility(false);
	Glyph_Animal->SetDesiredVisibility(false);
	Glyph_Stars->SetDesiredVisibility(false);
	Glyph_Darkness->SetDesiredVisibility(false);
	Glyph_Time->SetDesiredVisibility(false);

	//TheCityBelow_Text->SetVisibility(ESlateVisibility::Hidden);
	//Planetary_Text->SetVisibility(ESlateVisibility::Hidden);

	collectible1_Button->SetDisabled(true);
	collectible1_Button->SetDesiredVisibility(false);
	collectible2_Button->SetDisabled(true);
	collectible2_Button->SetDesiredVisibility(false);
	collectible3_Button->SetDisabled(true);
	collectible3_Button->SetDesiredVisibility(false);
	collectible4_Button->SetDisabled(true);
	collectible4_Button->SetDesiredVisibility(false);
	collectible5_Button->SetDisabled(true);
	collectible5_Button->SetDesiredVisibility(false);
	collectible6_Button->SetDisabled(true);
	collectible6_Button->SetDesiredVisibility(false);
	collectible7_Button->SetDisabled(true);
	collectible7_Button->SetDesiredVisibility(false);
	collectible8_Button->SetDisabled(true);
	collectible8_Button->SetDesiredVisibility(false);
	collectible9_Button->SetDisabled(true);
	collectible9_Button->SetDesiredVisibility(false);
	collectible10_Button->SetDisabled(true);
	collectible10_Button->SetDesiredVisibility(false);
	collectible11_Button->SetDisabled(true);
	collectible11_Button->SetDesiredVisibility(false);
	collectible12_Button->SetDisabled(true);
	collectible12_Button->SetDesiredVisibility(false);
	collectible13_Button->SetDisabled(true);
	collectible13_Button->SetDesiredVisibility(false);
	collectible14_Button->SetDisabled(true);
	collectible14_Button->SetDesiredVisibility(false);
	collectible15_Button->SetDisabled(true);
	collectible15_Button->SetDesiredVisibility(false);

	pagesMenu->Show();
	pagesSwitcher->SetActiveWidgetIndex(0);
	collectiblesMenu->Hide();
	settingsMenu->Hide();
	glyphsMenu->Hide();
	QuitMenu->Hide();
	FinalQuestionCanvas->Hide();
}

#pragma region Interaction Functions
void UBookPageWidget::MoveBookMouse(FVector2D _porcentage)
{
	UCanvasPanelSlot* circleSlot = Cast<UCanvasPanelSlot>(circleTest->Slot);
	FVector2D widgetSize(1920, 1080);
	//GEngine->GameViewport->GetViewportSize(widgetSize);
	//DustyUtils::PrintMessage("Viewport", widgetSize, FColor::Yellow, GetWorld()->DeltaTimeSeconds);

	//The mouse position on the UI
	FVector2D mousePos;
	mousePos.X = _porcentage.X * widgetSize.X;
	mousePos.Y = _porcentage.Y * widgetSize.Y;
	circleSlot->SetPosition(mousePos);

	if (m_currentButton && m_currentButton->IsBookButtonVisible())
	{
		bool mouseInside = CheckButtonBoundaries(m_currentButton, mousePos);
		if (!mouseInside)
		{
			SetCurrentButton(nullptr);
		}
	}
	else
	{
		for (int32 i = 0; i < m_buttons.Num(); i++)
		{
			if (m_buttons[i] && m_buttons[i]->IsBookButtonVisible())
			{
				bool mouseInside = CheckButtonBoundaries(m_buttons[i], mousePos);
				if (mouseInside)
				{
					SetCurrentButton(m_buttons[i]);
				}
			}
		}
	}
}

bool UBookPageWidget::CheckButtonBoundaries(UBookButton* _button, FVector2D& mousePos)
{
	UCanvasPanelSlot* buttonSlot = Cast<UCanvasPanelSlot>(_button->Slot);
	FGeometry WidgetGeometry = _button->GetCachedGeometry();
	FVector2D pos = WidgetGeometry.GetAbsolutePosition();
	FVector2D size = WidgetGeometry.Size;

	float left = pos.X;
	float right = pos.X + size.X;
	float top = pos.Y;
	float bottom = pos.Y + size.Y;
	return mousePos.X > left && mousePos.X < right&& mousePos.Y > top && mousePos.Y < bottom;
}

void UBookPageWidget::NavigatePageButtons(FVector2D _value)
{
	if (m_currentButton == nullptr)
	{
		if (m_firstButton)
		{
			m_currentButton = m_firstButton;
			DustyUtils::PrintMessage("SET NavigatePageButtons BUTTON: " + m_currentButton->GetName(), FColor::Red, 10);
		}
		else
		{
			//UE_LOG
			return;
		}
	}

	if (_value.X > 0 && m_currentButton->RightButton) // && !m_currentButton->RightButton->GetIsDisabled()
	{
		SetCurrentButton(m_currentButton->RightButton);
	}
	else if (_value.X < 0 && m_currentButton->LeftButton)
	{
		SetCurrentButton(m_currentButton->LeftButton);
	}
	else if (_value.Y > 0 && m_currentButton->UpButton)
	{
		SetCurrentButton(m_currentButton->UpButton);
	}
	else if (_value.Y < 0 && m_currentButton->DownButton)
	{
		SetCurrentButton(m_currentButton->DownButton);
	}
	else 
	{
		return;
	}

	//UPanelWidget* ParentWidget = m_currentButton->GetParent();
	//UBookPageWidget* ParentUserWidget = Cast<UBookPageWidget>(ParentWidget->GetOuter());
	//if (ParentUserWidget == this)
	//{
	//	//DustyUtils::PrintMessage("Same Page", FColor::Cyan, GetWorld()->GetDeltaSeconds());
	//}
	//else
	//{
	//	ParentUserWidget->SetCurrentButton(m_currentButton);
	//	//DustyUtils::PrintMessage("Diferent Page", FColor::Blue, 2);
	//}
}

void UBookPageWidget::PressCurrentButton()
{
	if (m_currentButton && m_currentButton->IsBookButtonVisible())
	{
		DustyUtils::PrintMessage(m_currentButton->GetName());
		m_currentButton->Pressed();
	}
}

void UBookPageWidget::ReleaseButton()
{
	if (m_currentButton)
	{
		DustyUtils::PrintMessage("Released:" + m_currentButton->GetName());
		if (m_currentButton->IsBookButtonVisible())
		{
			m_currentButton->Released();
		}
	}
}

void UBookPageWidget::ViusallyPressButton()
{
	if (m_currentButton && m_currentButton->IsBookButtonVisible())
	{
		m_currentButton->SetPressedColor();
	}
}

void UBookPageWidget::VisuallyRealeaseButton()
{
	if (m_currentButton && m_currentButton->IsBookButtonVisible())
	{
		m_currentButton->SetHighlightedColor();
	}
}
#pragma endregion

#pragma region LogBook Functions
void UBookPageWidget::LB_CreatePages()
{
	int maxPage = 0;

	for (int i = 0; i < PagesClasses.Num(); ++i)
	{
		UPageWidget* pageToAdd = CreateWidget<UPageWidget>(GetWorld(), PagesClasses[i]);
		pageToAdd->PageInit();

		pageToAdd->SetPageNumber(i);
		pagesSwitcher->AddChild(pageToAdd);

		if (i == 0 || i == 1 || !pageToAdd->GetIsLocked())
		{
			if (pageToAdd->GetPageNumber() > maxPage)
			{
				maxPage = pageToAdd->GetPageNumber();
			}
		}
		else
		{
			m_pagesLockedList.Add(pageToAdd);
		}

		m_pagesList.Add(pageToAdd);
		pageToAdd->Unlock();
	}

	BookActor->SetMaxPageUnlocked(maxPage);
}

void UBookPageWidget::LB_CreateCollectibles()
{
	for (int i = 0; i < CollectiblesClasses.Num(); ++i)
	{
		UPageWidget* collectibleToAdd = CreateWidget<UPageWidget>(GetWorld(), CollectiblesClasses[i]);
		collectibleToAdd->PageInit();

		collectiblesSwitcher->AddChild(collectibleToAdd);

		if (collectibleToAdd->GetIsLocked())
		{
			m_collectiblesLockedList.Add(collectibleToAdd);
		}
	}
}

void UBookPageWidget::LB_SetPage(int _page)
{
	DustyUtils::PrintMessage("MIAU");

	pagesSwitcher->SetActiveWidgetIndex(_page);	

	UPageWidget* PageWidget = Cast<UPageWidget>(pagesSwitcher->GetActiveWidget());
	if (PageWidget)
	{
		FText Title;
		FText Content;

		FString ContentTexts;

		TArray<UWidget*> CanvasChildren = PageWidget->Unlocked_Panel->GetAllChildren();
		int TextIndex = 0;
		for (UWidget* Widget : CanvasChildren)
		{
			UTextBlock* TextBlock = Cast<UTextBlock>(Widget);
			if (TextBlock)
			{
				if (TextIndex == 0)
				{
					Title = TextBlock->GetText();
				}
				else
				{
					ContentTexts += "\n\n" + TextBlock->GetText().ToString();
				}
			}
			TextIndex++;
		}

		Content = FText::FromString(ContentTexts);

		DustyUtils::SwapReadModeTexts(GetWorld(), Title, Content);
	}
}

void UBookPageWidget::LB_SetCollectibles(int _page, bool _taleSelectedButton)
{
	if (_page > 0)
	{
		BackToCollectibles_Button->SetDesiredVisibility(true);
		AllCollectiblesMenu->SetDesiredVisibility(false);
	}
	else
	{
		BackToCollectibles_Button->SetDesiredVisibility(false);
		AllCollectiblesMenu->SetDesiredVisibility(true);
	}

	collectiblesSwitcher->SetActiveWidgetIndex(_page);
	ECommonInputType InputType = DustyUtils::GetCurrentInputType();

	if (InputType == ECommonInputType::Gamepad)
	{
		if (_page > 0)
		{
			if (_taleSelectedButton && m_currentButton != nullptr)
			{
				lastCollectibleButtonSelected = m_currentButton;
			}
			SetCurrentButton(BackToCollectibles_Button);
		}
		else
		{
			if (lastCollectibleButtonSelected != nullptr)
			{
				SetCurrentButton(lastCollectibleButtonSelected);
				lastCollectibleButtonSelected = nullptr;
			}
			else
			{
				SetCurrentButton(collectiblesMenu->FirstButton);
			}
		}
	}
}

void UBookPageWidget::LB_SetAllCollectiblesMenuVisibility(bool _visibility)
{
	AllCollectiblesMenu->SetDesiredVisibility(_visibility);
}

void UBookPageWidget::LB_UnlockPage(int _pageNumber)
{
	int maxPage = BookActor->GetMaxPageUnlocked();

	for (int i = 0; i < m_pagesLockedList.Num(); ++i)
	{
		if (m_pagesLockedList[i]->GetPageNumber() == _pageNumber)
		{
			if (_pageNumber > maxPage)
			{
				maxPage = _pageNumber;
				BookActor->SetMaxPageUnlocked(maxPage);
			}
			m_pagesLockedList[i]->Unlock();
			return;
		}
	}

}

void UBookPageWidget::LB_UnlockPage(TSubclassOf<UPageWidget> _pageToUnlock)
{
	int maxPage = BookActor->GetMaxPageUnlocked();

	for (int i = 0; i < m_pagesLockedList.Num(); ++i)
	{
		if (m_pagesLockedList[i]->GetPageClass() == _pageToUnlock)
		{
			int page = m_pagesLockedList[i]->GetPageNumber();
			if (page > maxPage)
			{
				maxPage = page;
 				BookActor->SetMaxPageUnlocked(maxPage);
			}

			m_pagesLockedList[i]->Unlock();
			return;
		}
	}
}

void UBookPageWidget::LB_UnlockAllPages()
{
	int maxPage = BookActor->GetMaxPageUnlocked();
	for (int i = 0; i < m_pagesLockedList.Num(); ++i)
	{
		int page = m_pagesLockedList[i]->GetPageNumber();
		if (page > maxPage)
		{
			maxPage = page;
			BookActor->SetMaxPageUnlocked(maxPage);
		}

		m_pagesLockedList[i]->Unlock();		
	}
}

void UBookPageWidget::LB_LockPage(TSubclassOf<UPageWidget> _pageToUnlock)
{
	for (int i = 0; i < m_pagesList.Num(); i++)
	{
		if (m_pagesList[i]->GetPageClass() == _pageToUnlock)
		{
			BookActor->SetMaxPageUnlocked(m_pagesList[i]->GetPageNumber()-1);
			m_pagesList[i]->Lock();
			return;
		}
	}
}

void UBookPageWidget::LB_LockAllPages()
{
	//Page 0 and 1 are unlock by default on game start
	for (int i = 0; i < m_pagesList.Num(); i++)
	{
		if (i == 0 || i == 1)
		{
			m_pagesList[i]->Unlock();
		}
		m_pagesList[i]->Lock();
	}
	BookActor->SetMaxPageUnlocked(1);
}

void UBookPageWidget::LB_UnlockCollectible(TSubclassOf<UPageWidget> _pageToUnlock)
{
	for (int i = 0; i < m_collectiblesLockedList.Num(); ++i)
	{
		if (m_collectiblesLockedList[i]->GetPageClass() == _pageToUnlock)
		{
			m_collectiblesLockedList[i]->Unlock();
			EnableCollectibleButton(i);
			break;
		}
	}
	for (int i = 0; i < m_collectiblesLockedList.Num(); ++i)
	{
		if (m_collectiblesLockedList[i]->GetIsLocked())
		{
			return;
		}
	}

	ADustyCharacter* dusty = DustyUtils::GetDustyCharacter();
	if (dusty)
	{
		dusty->UnlockAchievement(TEXT("Collectibles_ACH"));
	}
	
}

void UBookPageWidget::LB_UnlockAllCollectibles()
{
	for (int i = 0; i < m_collectiblesLockedList.Num(); ++i)
	{
		m_collectiblesLockedList[i]->Unlock();
		EnableCollectibleButton(i);
	}
}

void UBookPageWidget::LB_LockCollectible(TSubclassOf<UPageWidget>& _pageToUnlock)
{
	for (int i = 0; i < m_collectiblesLockedList.Num(); ++i)
	{
		if (m_collectiblesLockedList[i]->GetPageClass() == _pageToUnlock)
		{
			m_collectiblesLockedList[i]->Lock();
			EnableCollectibleButton(i, false);
			return;
		}
	}
}

void UBookPageWidget::LB_LockAllCollectibles()
{
	for (int i = 0; i < m_collectiblesLockedList.Num(); ++i)
	{
		m_collectiblesLockedList[i]->Lock();
		EnableCollectibleButton(i, false);
	}	
	//TheCityBelow_Text->SetVisibility(ESlateVisibility::Hidden);
	//Planetary_Text->SetVisibility(ESlateVisibility::Hidden);
}

void UBookPageWidget::LB_UnlockGlyph(EGlyphs _glyphToUnlock)
{
	switch (_glyphToUnlock)
	{
	case EGlyphs::Road:
		//Already unlocked from the beginning
		break;
	case EGlyphs::Light:
		//Already unlocked from the beginning
		break;
	case EGlyphs::House:
		Glyph_House->SetDesiredVisibility(true);
		break;
	case EGlyphs::Person:
		Glyph_Person->SetDesiredVisibility(true);
		break;
	case EGlyphs::Animal:
		Glyph_Animal->SetDesiredVisibility(true);
		break;
	case EGlyphs::Stars:
		Glyph_Stars->SetDesiredVisibility(true);
		break;
	case EGlyphs::Darkness:
		Glyph_Darkness->SetDesiredVisibility(true);
		break;
	case EGlyphs::Time:
		Glyph_Time->SetDesiredVisibility(true);
		break;
	default:
		break;
	}
}

void UBookPageWidget::LB_LockGlyph(EGlyphs _glyphToUnlock)
{
	switch (_glyphToUnlock)
	{
	case EGlyphs::Road:
		//Already unlocked from the beginning
		break;
	case EGlyphs::Light:
		//Already unlocked from the beginning
		break;
	case EGlyphs::House:
		Glyph_House->SetDesiredVisibility(false);
		break;
	case EGlyphs::Person:
		Glyph_Person->SetDesiredVisibility(false);
		break;
	case EGlyphs::Animal:
		Glyph_Animal->SetDesiredVisibility(false);
		break;
	case EGlyphs::Stars:
		Glyph_Stars->SetDesiredVisibility(false);
		break;
	case EGlyphs::Darkness:
		Glyph_Darkness->SetDesiredVisibility(false);
		break;
	case EGlyphs::Time:
		Glyph_Time->SetDesiredVisibility(false);
		break;
	default:
		break;
	}
}

void UBookPageWidget::LB_LockAllGlyphs()
{
	Glyph_House->SetDesiredVisibility(false);
	Glyph_Person->SetDesiredVisibility(false);
	Glyph_Animal->SetDesiredVisibility(false);
	Glyph_Stars->SetDesiredVisibility(false);
	Glyph_Darkness->SetDesiredVisibility(false);
	Glyph_Time->SetDesiredVisibility(false);
}

void UBookPageWidget::EnableCollectibleButton(int i, bool _enable)
{
	UBookButton* collectible = GetCollectibleButton(i);
	collectible->SetDisabled(!_enable);
	collectible->SetDesiredVisibility(_enable);

	if (_enable)
	{
		//if (i >= 4 && i <= 9 && TheCityBelow_Text->GetVisibility() == ESlateVisibility::Hidden)
		//{
		//	TheCityBelow_Text->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		//}
		//else if (i >= 10 && Planetary_Text->GetVisibility() == ESlateVisibility::Hidden)
		//{
		//	Planetary_Text->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		//}

		for (int j = 0; j < i; j++)
		{
			UBookButton* button = GetCollectibleButton(j);

			if (button->GetDesiredVisibility() == false)
			{
				button->SetDisabled(true);
				button->SetDesiredVisibility(true);
			}
		}
	}
}

UBookButton* UBookPageWidget::GetCollectibleButton(int i)
{
	switch (i)
	{
		//WILDERNESS COLLECTIBLES
	case 0:
		return collectible1_Button;
		break;
	case 1:
		return collectible2_Button;
		break;
	case 2:
		return collectible3_Button;
		break;
	case 3:
		return collectible4_Button;
		break;

		//THE CITY BELOW COLLECTIBLES
	case 4:
		return collectible5_Button;
		break;
	case 5:
		return collectible6_Button;
		break;
	case 6:
		return collectible7_Button;
		break;
	case 7:
		return collectible8_Button;
		break;
	case 8:
		return collectible9_Button;
		break;
	case 9:
		return collectible10_Button;
		break;

		//PLANETARY COLLECTIBLES
	case 10:
		return collectible11_Button;
		break;
	case 11:
		return collectible12_Button;
		break;
	case 12:
		return collectible13_Button;
		break;
	case 13:
		return collectible14_Button;
		break;
	case 14:
		return collectible15_Button;
		break;

	default:
		return nullptr;
		break;
	}
	return nullptr;
}
#pragma endregion

#pragma region UI_Buttons_Functions
void UBookPageWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateSettingsButtons(InDeltaTime);
}


void UBookPageWidget::ResetCanUpdateValue()
{
	m_CanUpdateValue = true;
}

void UBookPageWidget::UpdateSettingsButtons(float DeltaTime)
{
	//If update in cooldown
	if (!m_CanUpdateValue)
	{
		return;
	}

	//If no buttons are pressed
	if (!(m_MusicLeftButtonPressed ||
		m_MusicRightButtonPressed ||
		m_EffectsLeftButtonPressed ||
		m_EffectsRightButtonPressed ||
		m_CameraSensitivityLeftButtonPressed ||
		m_CameraSensitivityRightButtonPressed ||
		m_GeneralLeftButtonPressed ||
		m_GeneralRightButtonPressed ||
		m_AimCameraSensitivityLeftButtonPressed ||
		m_AimCameraSensitivityRightButtonPressed))
	{
		return;
	}

	//Any button is pressed
	m_CanUpdateValue = false;
	GetWorld()->GetTimerManager().SetTimer(m_UpdateValueHandle, this, &UBookPageWidget::ResetCanUpdateValue, m_CooldownUpdateValue, false);


	if (m_MusicLeftButtonPressed)
	{
		if (m_NewMusicVolume >= ChangeAmount)
		{
			m_NewMusicVolume -= ChangeAmount;
		}
	}
	else if (m_MusicRightButtonPressed)
	{
		if (m_NewMusicVolume <= 100 - ChangeAmount)
		{
			m_NewMusicVolume += ChangeAmount;
		}
	}
	else if (m_EffectsLeftButtonPressed)
	{
		if (m_NewEffectsVolume >= ChangeAmount)
		{
			m_NewEffectsVolume -= ChangeAmount;
		}
	}
	else if (m_EffectsRightButtonPressed)
	{
		if (m_NewEffectsVolume <= 100 - ChangeAmount)
		{
			m_NewEffectsVolume += ChangeAmount;
		}
	}
	else if (m_CameraSensitivityLeftButtonPressed)
	{
		if (m_NewCameraSensitivity >= ChangeAmount + 1)
		{
			m_NewCameraSensitivity -= ChangeAmount;
		}
	}
	else if (m_CameraSensitivityRightButtonPressed)
	{
		if (m_NewCameraSensitivity <= 200 - ChangeAmount)
		{
			m_NewCameraSensitivity += ChangeAmount;
		}
	}
	else if (m_GeneralLeftButtonPressed)
	{
		if (m_NewGeneralVolume >= ChangeAmount)
		{
			m_NewGeneralVolume -= ChangeAmount;
		}
	}
	else if (m_GeneralRightButtonPressed)
	{
		if (m_NewGeneralVolume <= 100 - ChangeAmount)
		{
			m_NewGeneralVolume += ChangeAmount;
		}
	}
	else if (m_AimCameraSensitivityLeftButtonPressed)
	{
		if (m_NewAimCameraSensitivity >= ChangeAmount + 1)
		{
			m_NewAimCameraSensitivity -= ChangeAmount;
		}
	}
	else if (m_AimCameraSensitivityRightButtonPressed)
	{
		if (m_NewAimCameraSensitivity <= 200 - ChangeAmount)
		{
			m_NewAimCameraSensitivity += ChangeAmount;
		}
	}

	General_Text->SetText(FText::FromString(FString::FromInt(m_NewGeneralVolume)));
	Music_Text->SetText(FText::FromString(FString::FromInt(m_NewMusicVolume)));
	Effect_Text->SetText(FText::FromString(FString::FromInt(m_NewEffectsVolume)));
	CameraSensitivity_Text->SetText(FText::FromString(FString::FromInt(m_NewCameraSensitivity)));
	AimCameraSensitivity_Text->SetText(FText::FromString(FString::FromInt(m_NewAimCameraSensitivity)));

	if (AuxiliarWidget)
	{
		AuxiliarWidget->General_Text->SetText(FText::FromString(FString::FromInt(m_NewGeneralVolume)));
		AuxiliarWidget->Music_Text->SetText(FText::FromString(FString::FromInt(m_NewMusicVolume)));
		AuxiliarWidget->Effect_Text->SetText(FText::FromString(FString::FromInt(m_NewEffectsVolume)));
		AuxiliarWidget->CameraSensitivity_Text->SetText(FText::FromString(FString::FromInt(m_NewCameraSensitivity)));
		AuxiliarWidget->AimCameraSensitivity_Text->SetText(FText::FromString(FString::FromInt(m_NewAimCameraSensitivity)));
	}
}

void UBookPageWidget::TryToChangeSettings()
{
	m_NewDisplayMode = m_OriginalDisplayMode;
	m_NewResolution = m_OriginalResolution;
	m_NewGeneralVolume = m_OriginalGeneralVolume;
	m_NewMusicVolume = m_OriginalMusicVolume;
	m_NewEffectsVolume = m_OriginalEffectsVolume;
	m_NewCameraSensitivity = m_OriginalCameraSensitivity;
	m_NewAimCameraSensitivity = m_OriginalAimCameraSensitivity;

	General_Text->SetText(FText::FromString(FString::FromInt(m_NewGeneralVolume)));
	Music_Text->SetText(FText::FromString(FString::FromInt(m_NewMusicVolume)));
	Effect_Text->SetText(FText::FromString(FString::FromInt(m_NewEffectsVolume)));
	CameraSensitivity_Text->SetText(FText::FromString(FString::FromInt(m_NewCameraSensitivity)));
	AimCameraSensitivity_Text->SetText(FText::FromString(FString::FromInt(m_NewAimCameraSensitivity)));

	if (AuxiliarWidget)
	{
		AuxiliarWidget->General_Text->SetText(FText::FromString(FString::FromInt(m_NewGeneralVolume)));
		AuxiliarWidget->Music_Text->SetText(FText::FromString(FString::FromInt(m_NewMusicVolume)));
		AuxiliarWidget->Effect_Text->SetText(FText::FromString(FString::FromInt(m_NewEffectsVolume)));
		AuxiliarWidget->CameraSensitivity_Text->SetText(FText::FromString(FString::FromInt(m_NewCameraSensitivity)));
		AuxiliarWidget->AimCameraSensitivity_Text->SetText(FText::FromString(FString::FromInt(m_NewAimCameraSensitivity)));
	}
}


void UBookPageWidget::B_BackToCollectibles()
{
	BookActor->LB_ReturnToCollectiblesMenu();
	DustyUtils::HideHudWithNoFade(GetWorld(), EHudCanvas::ReadMode);
	DustyUtils::HideHudWithNoFade(GetWorld(), EHudCanvas::ReadModeButton);

	ECommonInputType InputType = DustyUtils::GetCurrentInputType();
	if (InputType == ECommonInputType::Gamepad)
	{
		SetCurrentButton(collectiblesMenu->FirstButton);
	}
	BookActor->EnableReadMode(false);
}

void UBookPageWidget::B_QuitGame()
{
	ADustyCharacter* Dusty = DustyUtils::GetDustyCharacter();

	m_currentButton = nullptr;
	BookActor->lookForMouse = false;

	DustyUtils::PrintMessage("AWJVDFCVSAJFVSADBVFSJDFSJDVBFSDJFSBDFBJSDJFSJDFBSDF");
	if (Dusty)
	{
		Dusty->m_MainMenuInstance->OpenMainMenu();
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		PlayerController->bShowMouseCursor = true;
	}
	/*SetCurrentButton(QuitMenu->FirstButton);
	QuitMenu->Show();
	settingsMenu->Hide();*/
}

void UBookPageWidget::B_QuitGame_Yes()
{
	//if (askedToQuit)
	//{
		// PLACEHOLDER
		//This should call a manager that handles how we quit the game
		UKismetSystemLibrary::QuitGame(GetWorld(), NULL, EQuitPreference::Quit, true);
	//}
}

void UBookPageWidget::B_QuitGame_No()
{
	QuitMenu->Hide();
	settingsMenu->Show();
}

void UBookPageWidget::B_ShowLastCinematicNoQuest()
{
	UCinematicsSubsystem* cinematicSubsystem = GetGameInstance()->GetSubsystem<UCinematicsSubsystem>();
	if (cinematicSubsystem)
	{
		cinematicSubsystem->OnEndCinematic.Clear();
		ADustyCharacter* dusty = DustyUtils::GetDustyCharacter();

		dusty->ActionsFilterComponent->SetCurrentState(UMainMenuState::StaticClass());
		dusty->UnlockAchievement(TEXT("No_ACH"));
		dusty->SetActorHiddenInGame(true);
		cinematicSubsystem->OnEndCinematic.AddDynamic(dusty, &ADustyCharacter::ShowCredits);
		cinematicSubsystem->PlayCinematic(cinematicSubsystem->ShowFinalSequenceNoQuest);
	}
}

void UBookPageWidget::B_ShowLastCinematicQuest()
{
	UCinematicsSubsystem* cinematicSubsystem = GetGameInstance()->GetSubsystem<UCinematicsSubsystem>();
	if (cinematicSubsystem)
	{
		cinematicSubsystem->OnEndCinematic.Clear();
		ADustyCharacter* dusty = DustyUtils::GetDustyCharacter();

		dusty->ActionsFilterComponent->SetCurrentState(UMainMenuState::StaticClass());
		dusty->UnlockAchievement(TEXT("Yes_ACH"));
		dusty->SetActorHiddenInGame(true);
		cinematicSubsystem->OnEndCinematic.AddDynamic(dusty, &ADustyCharacter::ShowCredits);
		cinematicSubsystem->PlayCinematic(cinematicSubsystem->ShowFinalSequenceQuest);
	}
}

void UBookPageWidget::GoToCollectible(int PageNumber)
{
	BookActor->LB_GoToCollectible(PageNumber);
	BookActor->EnableReadMode(true);
}

//void UBookPageWidget::B_Glyph_Road()
//{
//	glyphsSwitcher->SetActiveWidgetIndex(1);
//}
//
//void UBookPageWidget::B_Glyph_Light()
//{
//	glyphsSwitcher->SetActiveWidgetIndex(2);
//}
//
//void UBookPageWidget::B_Glyph_House()
//{
//	glyphsSwitcher->SetActiveWidgetIndex(3);
//}
//
//void UBookPageWidget::B_Glyph_Person()
//{
//	glyphsSwitcher->SetActiveWidgetIndex(4);
//}
//
//void UBookPageWidget::B_Glyph_Animal()
//{
//	glyphsSwitcher->SetActiveWidgetIndex(5);
//}
//
//void UBookPageWidget::B_Glyph_Stars()
//{
//	glyphsSwitcher->SetActiveWidgetIndex(6);
//}
//
//void UBookPageWidget::B_Glyph_Darkness()
//{
//	glyphsSwitcher->SetActiveWidgetIndex(7);
//}
//
//void UBookPageWidget::B_Glyph_Time()
//{
//	glyphsSwitcher->SetActiveWidgetIndex(8);
//}
#pragma endregion

#pragma region Settings
void UBookPageWidget::B_DisplayModeLeft()
{
	// Change display mode in text
	ChangeDisplayMode(false);

	// Set the new display mode in the game
	DisplayModeSetting();
}

void UBookPageWidget::B_DisplayModeRight()
{
	// Change display mode in text
	ChangeDisplayMode(true);

	// Set the new display mode in the game
	DisplayModeSetting();
}

void UBookPageWidget::DisplayModeSetting()
{
	//// Get the settings subsystem
	//USettingsSubsystem* settingsSubsystem = GetGameInstance()->GetSubsystem<USettingsSubsystem>();

	//FString DisplayType = DisplayMode_Text->GetText().ToString();

	//// Set the new diaplsy mode in PlayerSettings
	//settingsSubsystem->SetDisplayMode(DisplayType);
}

void UBookPageWidget::ChangeDisplayMode(bool IsRightButton)
{
	if (IsRightButton)
	{
		// Move to the right (next display mode)
		m_CurrentDisplayMode = (m_CurrentDisplayMode + 1) % m_DisplayModes.Num();
	}
	else
	{
		// Move to the left (previous mode)
		m_CurrentDisplayMode = (m_CurrentDisplayMode - 1 + m_DisplayModes.Num()) % m_DisplayModes.Num();
	}

	USettingsSubsystem* settingsSubsystem = GetGameInstance()->GetSubsystem<USettingsSubsystem>();

	// Update the text with the new display mode
	DisplayMode_Text->SetText(FText::FromString(m_DisplayModes[m_CurrentDisplayMode]));

	if (AuxiliarWidget)
	{
		AuxiliarWidget->DisplayMode_Text->SetText(FText::FromString(m_DisplayModes[m_CurrentDisplayMode]));
	}
}

void UBookPageWidget::B_ResolutionLeft()
{
	// Change resolution in text box
	ChangeResolution(false);

	// Set the new resolution in the game
	ResolutionSetting();
}

void UBookPageWidget::B_ResolutionRight()
{
	// Change resolution in text box
	ChangeResolution(true);

	// Set the new resolution in the game
	ResolutionSetting();
}

void UBookPageWidget::B_GeneralVolumeLeftPressed()
{
	m_GeneralLeftButtonPressed = true;
	m_GeneralRightButtonPressed = false;
}

void UBookPageWidget::B_GeneralVolumeLeftReleased()
{
	m_GeneralLeftButtonPressed = false;
}

void UBookPageWidget::B_GeneralVolumeRightPressed()
{
	m_GeneralRightButtonPressed = true;
	m_GeneralLeftButtonPressed = false;
}

void UBookPageWidget::B_GeneralVolumeRightReleased()
{
	m_GeneralRightButtonPressed = false;
}

void UBookPageWidget::B_MusicVolumeLeftPressed()
{
	m_MusicLeftButtonPressed = true;
	m_MusicRightButtonPressed = false;
}

void UBookPageWidget::B_MusicVolumeLeftReleased()
{
	m_MusicLeftButtonPressed = false;
}

void UBookPageWidget::ResolutionSetting()
{
	//// Get the settings subsystem
	//USettingsSubsystem* settingsSubsystem = GetGameInstance()->GetSubsystem<USettingsSubsystem>();

	//// Obtain the resolutio text selected
	//FString Resolution = Resolution_Text->GetText().ToString();

	//// Variables to store screen width and heigth
	//int ResolutionWidth = 0;
	//int ResolutionHeigth = 0;

	//// Find the 'x' position and split the char
	//FString LeftStr;
	//FString RightStr;

	//if (Resolution.Split(TEXT(" x "), &LeftStr, &RightStr))
	//{
	//	// Transform parts to float
	//	ResolutionWidth = FCString::Atoi(*LeftStr);
	//	ResolutionHeigth = FCString::Atoi(*RightStr);
	//}

	//settingsSubsystem->SetResolution(ResolutionWidth, ResolutionHeigth);
}

void UBookPageWidget::ChangeResolution(bool IsRightButton)
{
	if (IsRightButton)
	{
		// Move to the right (next display mode)
		m_CurrentResolution = (m_CurrentResolution + 1) % m_Resolutions.Num();
	}
	else
	{
		// Move to the left (previous mode)
		m_CurrentResolution = (m_CurrentResolution - 1 + m_Resolutions.Num()) % m_Resolutions.Num();
	}

	// Update the text with the new display mode
	FText resText = FText::FromString(m_Resolutions[m_CurrentResolution]);
	Resolution_Text->SetText(resText);
	if (AuxiliarWidget)
	{
		AuxiliarWidget->Resolution_Text->SetText(resText);
	}
}


void UBookPageWidget::B_MusicVolumeRightPressed()
{
	m_MusicRightButtonPressed = true;
	m_MusicLeftButtonPressed = false;
}

void UBookPageWidget::B_MusicVolumeRightReleased()
{
	m_MusicRightButtonPressed = false;
}

void UBookPageWidget::B_EffectsVolumeLeftPressed()
{
	m_EffectsLeftButtonPressed = true;
	m_EffectsRightButtonPressed = false;
}

void UBookPageWidget::B_EffectsVolumeLeftReleased()
{
	m_EffectsLeftButtonPressed = false;
}

void UBookPageWidget::B_EffectsVolumeRightPressed()
{
	m_EffectsRightButtonPressed = true;
	m_EffectsLeftButtonPressed = false;
}

void UBookPageWidget::B_EffectsVolumeRightReleased()
{
	m_EffectsRightButtonPressed = false;
}

void UBookPageWidget::B_CameraSensitivityLeftPressed()
{
	m_CameraSensitivityLeftButtonPressed = true;
	m_CameraSensitivityRightButtonPressed = false;
}

void UBookPageWidget::B_CameraSensitivityLeftReleased()
{
	m_CameraSensitivityLeftButtonPressed = false;
}

void UBookPageWidget::B_CameraSensitivityRightPressed()
{
	m_CameraSensitivityRightButtonPressed = true;
	m_CameraSensitivityLeftButtonPressed = false;
}

void UBookPageWidget::B_CameraSensitivityRightReleased()
{
	m_CameraSensitivityRightButtonPressed = false;
}

void UBookPageWidget::B_AimCameraSensitivityLeftPressed()
{
	m_AimCameraSensitivityLeftButtonPressed = true;
	m_AimCameraSensitivityRightButtonPressed = false;
}

void UBookPageWidget::B_AimCameraSensitivityLeftReleased()
{
	m_AimCameraSensitivityLeftButtonPressed = false;
}

void UBookPageWidget::B_AimCameraSensitivityRightPressed()
{
	m_AimCameraSensitivityRightButtonPressed = true;
	m_AimCameraSensitivityLeftButtonPressed = false;
}

void UBookPageWidget::B_AimCameraSensitivityRightReleased()
{
	m_AimCameraSensitivityRightButtonPressed = false;
}

void UBookPageWidget::B_ApplyChanges()
{


	 m_OriginalMusicVolume = m_NewMusicVolume;
	 m_OriginalEffectsVolume = m_NewEffectsVolume;
	 m_OriginalCameraSensitivity = m_NewCameraSensitivity;
	 m_OriginalGeneralVolume = m_NewGeneralVolume;
	 m_OriginalAimCameraSensitivity = m_NewAimCameraSensitivity;

	// Get the settings subsystem
	USettingsSubsystem* settingsSubsystem = GetGameInstance()->GetSubsystem<USettingsSubsystem>();

	settingsSubsystem->SetDisplayMode(m_DisplayModes[m_CurrentDisplayMode]);

	
	// Variables to store screen width and heigth
	int ResolutionWidth = 0;
	int ResolutionHeigth = 0;

	// Find the 'x' position and split the char
	FString LeftStr;
	FString RightStr;

	if (m_Resolutions[m_CurrentResolution].Split(TEXT(" x "), &LeftStr, &RightStr))
	{
		// Transform parts to float
		ResolutionWidth = FCString::Atoi(*LeftStr);
		ResolutionHeigth = FCString::Atoi(*RightStr);
	}
	settingsSubsystem->SetResolution(ResolutionWidth, ResolutionHeigth);
	settingsSubsystem->SetMusicVolume(m_OriginalMusicVolume);
	settingsSubsystem->SetEffectsVolume(m_OriginalEffectsVolume);
	settingsSubsystem->SetCameraSensitivity(m_OriginalCameraSensitivity);
	settingsSubsystem->SetAimCameraSensitivity(m_OriginalAimCameraSensitivity);
	settingsSubsystem->SetGeneralVolume(m_OriginalGeneralVolume);

	settingsSubsystem->SetSettingsChangesApplied(true);

	UGameUserSettings* Settings = UGameUserSettings::GetGameUserSettings();
	Settings->SaveSettings();
}

void UBookPageWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UDustyGameInstance* GameInstance = Cast<UDustyGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		FSettingsSaveData SettingsSaveData = GameInstance->SettingsSaveData;
		UGameUserSettings* Settings = UGameUserSettings::GetGameUserSettings();



		FString CurrentResolutionText = FString(FString::FromInt(SettingsSaveData.Resolution.X) + " x " + FString::FromInt(SettingsSaveData.Resolution.Y));
		//// Obtain the resolutio text selected
		//Resolution_Text->SetText(FText::FromString(CurrentResolutionText));


		m_CurrentDisplayMode = m_DisplayModes.Find(SettingsSaveData.DisplayMode);
		m_CurrentResolution = m_Resolutions.Find(CurrentResolutionText);
		DisplayMode_Text->SetText(FText::FromString(SettingsSaveData.DisplayMode));
		Resolution_Text->SetText(FText::FromString(CurrentResolutionText));
		//Music_Text->SetText(FText::FromString(FString::FromInt(SettingsSaveData.MusicVolume)));
		//Effect_Text->SetText(FText::FromString(FString::FromInt(SettingsSaveData.EffectsVolume)));
		//CameraSensitivity_Text->SetText(FText::FromString(FString::FromInt(SettingsSaveData.CameraSensitivity)));
		//AimCameraSensitivity_Text->SetText(FText::FromString(FString::FromInt(SettingsSaveData.AimCameraSensitivity)));
		//General_Text->SetText(FText::FromString(FString::FromInt(SettingsSaveData.GeneralVolume)));


		m_OriginalDisplayMode = SettingsSaveData.DisplayMode;
		m_OriginalResolution = FString(SettingsSaveData.Resolution.X + " x " + SettingsSaveData.Resolution.Y);
		m_OriginalMusicVolume = SettingsSaveData.MusicVolume;
		m_OriginalEffectsVolume = SettingsSaveData.EffectsVolume;
		m_OriginalCameraSensitivity = SettingsSaveData.CameraSensitivity;
		m_OriginalGeneralVolume = SettingsSaveData.GeneralVolume;
		m_OriginalAimCameraSensitivity = SettingsSaveData.AimCameraSensitivity;

		// Get the settings subsystem
		USettingsSubsystem* settingsSubsystem = GetGameInstance()->GetSubsystem<USettingsSubsystem>();

		settingsSubsystem->SetResolution(SettingsSaveData.Resolution.X, SettingsSaveData.Resolution.Y);
		settingsSubsystem->SetDisplayMode(SettingsSaveData.DisplayMode);
		settingsSubsystem->SetMusicVolume(SettingsSaveData.MusicVolume);
		settingsSubsystem->SetEffectsVolume(SettingsSaveData.EffectsVolume);
		settingsSubsystem->SetCameraSensitivity(SettingsSaveData.CameraSensitivity);
		settingsSubsystem->SetAimCameraSensitivity(SettingsSaveData.AimCameraSensitivity);
		settingsSubsystem->SetGeneralVolume(SettingsSaveData.GeneralVolume);

		settingsSubsystem->SetSettingsChangesApplied(true, false);
	}



}

void UBookPageWidget::CacheCurrentSettings()
{

}

void UBookPageWidget::RestoresOriginalSettings()
{
	
}

#pragma endregion