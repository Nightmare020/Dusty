// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../../../Utils/DustyConst.h"
#include <Components/TextBlock.h>
#include "BookWidget.generated.h"

class UPageWidget;
class UImage;
class UCanvasPanel;
class UButton;
class UWidgetSwitcher;

/**
 * 
 */
UCLASS()
class DUSTY_API UBookWidget : public UUserWidget
{
	GENERATED_BODY()

//Variables
private:
	TArray<UPageWidget*> m_pagesLockedList;
	int m_currentPage = 0;
	int m_maxPageNumberUnlocked = 0; //The biggest value of page number of all the UPageWidgets that are currently unlocked
	bool m_isOpen = false;
	bool m_tutorialOpen = false;

	//Functions
	void NativeConstruct() override;

	//Logbook
	void LB_CreatePages();
	void LB_NextPage();
	void LB_PreviousPage();

public:
	// All UUserWidgets added to this array should inherit from
	UPROPERTY(EditAnywhere, category = "Book")
	TArray<TSubclassOf<UPageWidget>> PagesClasses;

	//Functions
	void OpenBook(LG_Bookmarks _bookmark);
	void CloseBook();
	void ToggleBook();

	//LogBook
	void LB_UnlockPage(int _pageNumber);
	UFUNCTION(BlueprintCallable)
	void LB_UnlockPage(TSubclassOf<UPageWidget> _pageToUnlock);

	// Inputs Control Tutorial
	void ToggleControlsTutorial();

#pragma region UI_Panels_Variables
	//Canvas containing PauseMenu and Pages
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* Book;

	//Canvas containing the pause buttons
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* PauseMenu;

	//Canvas containing Both Pages
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* Pages;

	//Canvas containing hieroglyphics
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* GlyphsMenu;

	//Canvas containing hieroglyphics names
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* GlyphsNames;

	//Canvas containing tutorial controls
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* InputsTutorialShowed;

	//Canvas hiding tutorial controls
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* InputsTutorialHidden;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UWidgetSwitcher* LeftPage;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UWidgetSwitcher* RightPage;
#pragma endregion

#pragma region UI_Buttons_Variables
	//Pause Buttons
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* Resume_Button;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* LogBook_Button;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* Glyphs_Button;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* Quit_Button;

	//LogBook Buttons
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* NextPage_Button;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* PreviousPage_Button;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* ReturnPause_Button;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* ShowNames_Button;

#pragma endregion

	//TEMP FOR THE FP
#pragma region UIGlyphs 
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* LightImg;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* LightGlyph;
	void ShowGlyphLight();

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* SkyImg;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* SkyGlyph;
	void ShowGlyphSky();
#pragma endregion

#pragma region UI_Buttons_Functions
	//One function per button
	UFUNCTION()
	void B_Resume();
	UFUNCTION()
	void B_Logbook();
	UFUNCTION()
	void B_Glyphs();
	UFUNCTION()
	void B_Quit();
	UFUNCTION()
	void B_NextPage();
	UFUNCTION()
	void B_PreviousPage();
	UFUNCTION()
	void B_GoPause();
	UFUNCTION()
	void B_ShowGlyphsNames();
#pragma endregion
};
