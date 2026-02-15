// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../../Utils/DustyConst.h"
#include "Blueprint/UserWidget.h"

#include "BookPageWidget.generated.h"

class UPageWidget;
class ABookActorTextures;
class UDustyCanvasPanel;
class UBookButton;
class UWidgetSwitcher;
class ABookRenders;
class UTextBlock;
class USlider;

class UImage;

struct collectibleButton
{
	TSubclassOf<UPageWidget> collectibleType;
	UBookButton* button;
};

UCLASS()
class DUSTY_API UBookPageWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeConstruct() override;


	TArray<UPageWidget*> m_pagesLockedList;
	TArray<UPageWidget*> m_pagesList;
	TArray<UPageWidget*> m_collectiblesLockedList;

	ABookActorTextures* BookActor = nullptr;
	UBookPageWidget* AuxiliarWidget = nullptr;
	UBookButton* m_firstButton = nullptr;
	UBookButton* m_currentButton = nullptr;

	bool askedToQuit = false;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UDustyCanvasPanel* pagesMenu;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UWidgetSwitcher* pagesSwitcher;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UDustyCanvasPanel* settingsMenu;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UDustyCanvasPanel* collectiblesMenu;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UWidgetSwitcher* collectiblesSwitcher;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UDustyCanvasPanel* AllCollectiblesMenu;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UDustyCanvasPanel* glyphsMenu;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UDustyCanvasPanel* QuitMenu;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UDustyCanvasPanel* FinalQuestionCanvas;

	//Tests
	UPROPERTY(meta = (BindWidget))
	UImage* circleTest;

	UPROPERTY(EditDefaultsOnly)
	TArray<UBookButton*> m_buttons;

#pragma region UI_Glyphs_Variables
	//Collectibles Buttons
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UDustyCanvasPanel* Glyph_Road;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UDustyCanvasPanel* Glyph_Light;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UDustyCanvasPanel* Glyph_House;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UDustyCanvasPanel* Glyph_Person;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UDustyCanvasPanel* Glyph_Animal;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UDustyCanvasPanel* Glyph_Stars;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UDustyCanvasPanel* Glyph_Darkness;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UDustyCanvasPanel* Glyph_Time;
#pragma endregion

#pragma region UI_Buttons_Variables
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UBookButton* BackToCollectibles_Button;
public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UBookButton* Quit_Button;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UBookButton* Quit_Button_Yes;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UBookButton* Quit_Button_No;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UBookButton* FinalQuestionButtonYes;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UBookButton* FinalQuestionButtonNo;

	//UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	//	UTextBlock* TheCityBelow_Text;
	//UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	//	UTextBlock* Planetary_Text;


	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UBookButton* collectible1_Button;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UBookButton* collectible2_Button;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UBookButton* collectible3_Button;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UBookButton* collectible4_Button;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UBookButton* collectible5_Button;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UBookButton* collectible6_Button;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UBookButton* collectible7_Button;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UBookButton* collectible8_Button;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UBookButton* collectible9_Button;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UBookButton* collectible10_Button;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UBookButton* collectible11_Button;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UBookButton* collectible12_Button;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UBookButton* collectible13_Button;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UBookButton* collectible14_Button;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UBookButton* collectible15_Button;

	UPROPERTY(Transient)
		UBookButton* lastCollectibleButtonSelected = nullptr;
#pragma endregion

#pragma region UI_Settings_Variables
public:
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidget))
	UBookButton* ApplyChanges_Button;

	/** Select display mode buttons */
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidget))
	UBookButton* DisplayModeLeft_Button;

	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidget))
	UBookButton* DisplayModeRight_Button;

	/** Text Box with the possible different game display modes */
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidget))
	UTextBlock* DisplayMode_Text;

	/** Select display mode buttons */
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidget))
	UBookButton* ResolutionLeft_Button;

	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidget))
	UBookButton* ResolutionRight_Button;

	/** Text Box with the possible different game resolutions */
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidget))
	UTextBlock* Resolution_Text;


	/** Dicrease General sound */
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidget))
		UBookButton* GeneralLeft_Button;

	/** Increase General sound */
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidget))
		UBookButton* GeneralRight_Button;

	/** Text Box with General sound */
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidget))
		UTextBlock* General_Text;

	/** Dicrease music sound */
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidget))
		UBookButton* MusicLeft_Button;

	/** Increase music sound */
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidget))
		UBookButton* MusicRight_Button;

	/** Text Box with music sound */
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidget))
		UTextBlock* Music_Text;

	/** Dicrease effect sound */
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidget))
		UBookButton* EffectLeft_Button;

	/** Increase effect sound */
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidget))
		UBookButton* EffectRight_Button;

	/** Text Box with effect sound */
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidget))
		UTextBlock* Effect_Text;
	

	/** Dicrease CameraSensitivity sound */
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidget))
		UBookButton* CameraSensitivityLeft_Button;

	/** Increase CameraSensitivity sound */
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidget))
		UBookButton* CameraSensitivityRight_Button;

	/** Text Box with CameraSensitivity*/
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidget))
		UTextBlock* CameraSensitivity_Text;

	/** Dicrease AimCameraSensitivity sound */
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidget))
		UBookButton* AimCameraSensitivityLeft_Button;

	/** Increase AimCameraSensitivity sound */
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidget))
		UBookButton* AimCameraSensitivityRight_Button;

	/** Text Box with AimCameraSensitivity*/
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidget))
		UTextBlock* AimCameraSensitivity_Text;

	/** Array for all the possible display modes */
	TArray<FString> m_DisplayModes = { "Windowed", "Fullscreen", "Windowed Fullscreen" };

	/** Index for the current display mode */
	int32 m_CurrentDisplayMode = 2; // Fullscreen display mode as default display mode

	/** Array for all the possible resolutions */
	TArray<FString> m_Resolutions = { "1280 x 720", "1366 x 768", "1560 x 1080", "1920 x 1080","1920 x 1200" , "2560 x 1440", "3840 x 2160", "5120 x 1440", "7680 x 4320" };

	/** Index for the current resolution mode */
	int32 m_CurrentResolution = 2; // 1920 x 1080 as default resolution

private:
	/** This values are cached as the original settings values in ase we cancel the apply settings operation */
	FString m_OriginalDisplayMode = "Windowed Fullscreen";
	FString m_OriginalResolution = "1920 x 1080";
	int32 m_OriginalMusicVolume = 100;
	int32 m_OriginalGeneralVolume = 100;
	int32 m_OriginalEffectsVolume = 100;
	int32 m_OriginalCameraSensitivity = 100;
	int32 m_OriginalAimCameraSensitivity = 100;

	FString m_NewDisplayMode;
	FString m_NewResolution;
	int32 m_NewGeneralVolume;
	int32 m_NewMusicVolume;
	int32 m_NewEffectsVolume;
	int32 m_NewCameraSensitivity;
	int32 m_NewAimCameraSensitivity;

	bool m_MusicLeftButtonPressed = false;
	bool m_MusicRightButtonPressed = false;
	bool m_GeneralLeftButtonPressed = false;
	bool m_GeneralRightButtonPressed = false;
	bool m_EffectsLeftButtonPressed = false;
	bool m_EffectsRightButtonPressed = false;
	bool m_CameraSensitivityLeftButtonPressed = false;
	bool m_CameraSensitivityRightButtonPressed = false;
	bool m_AimCameraSensitivityLeftButtonPressed = false;
	bool m_AimCameraSensitivityRightButtonPressed = false;

	FTimerHandle m_UpdateValueHandle;
	float m_CooldownUpdateValue = 0.15;

	int ChangeAmount = 5;

	bool m_CanUpdateValue = true;

	void ResetCanUpdateValue();
	void UpdateSettingsButtons(float DeltaTime);
	void TryToChangeSettings();



#pragma endregion

#pragma region UI_Buttons_Functions
private:
	//One function per button
	UFUNCTION()
	void B_BackToCollectibles();
	UFUNCTION()
	void B_QuitGame();
	UFUNCTION()
	void B_QuitGame_Yes();
	UFUNCTION()
	void B_QuitGame_No();
	UFUNCTION()
	void B_ShowLastCinematicNoQuest();
	UFUNCTION()
	void B_ShowLastCinematicQuest();

public:
	UFUNCTION(BlueprintCallable) //This is meant to be used only in blueprint by the collectibles buttons
	void GoToCollectible(int PageNumber);
#pragma endregion

#pragma region UI_Settings_Functions
public:
	/** Settings Functions */
	UFUNCTION()
	void B_DisplayModeLeft();
	UFUNCTION()
	void B_DisplayModeRight();

	UFUNCTION()
	void B_ResolutionLeft();
	UFUNCTION()
	void B_ResolutionRight();

	UFUNCTION()
	void B_GeneralVolumeLeftPressed();
	UFUNCTION()
	void B_GeneralVolumeLeftReleased();
	UFUNCTION()
	void B_GeneralVolumeRightPressed();
	UFUNCTION()
	void B_GeneralVolumeRightReleased();

	UFUNCTION()
	void B_MusicVolumeLeftPressed();
	UFUNCTION()
	void B_MusicVolumeLeftReleased();
	UFUNCTION()
	void B_MusicVolumeRightPressed();
	UFUNCTION()
	void B_MusicVolumeRightReleased();
	UFUNCTION()
	void B_EffectsVolumeLeftPressed();
	UFUNCTION()
	void B_EffectsVolumeLeftReleased();
	UFUNCTION()
	void B_EffectsVolumeRightPressed();
	UFUNCTION()
	void B_EffectsVolumeRightReleased();
	UFUNCTION()
	void B_CameraSensitivityLeftPressed();
	UFUNCTION()
	void B_CameraSensitivityLeftReleased();
	UFUNCTION()
	void B_CameraSensitivityRightPressed();
	UFUNCTION()
	void B_CameraSensitivityRightReleased();

	UFUNCTION()
	void B_AimCameraSensitivityLeftPressed();
	UFUNCTION()
	void B_AimCameraSensitivityLeftReleased();
	UFUNCTION()
	void B_AimCameraSensitivityRightPressed();
	UFUNCTION()
	void B_AimCameraSensitivityRightReleased();

	UFUNCTION()
	void B_ApplyChanges();


private:
	void DisplayModeSetting();
	void ChangeDisplayMode(bool IsRightButton);

	void ResolutionSetting();
	void ChangeResolution(bool IsRightButton);

	void CacheCurrentSettings();
	void RestoresOriginalSettings();
#pragma endregion

public:
	// All UUserWidgets added to this array should inherit from
	UPROPERTY(EditAnywhere, category = "Book")
	TArray<TSubclassOf<UPageWidget>> PagesClasses;
	UPROPERTY(EditAnywhere, category = "Book")
	TArray<TSubclassOf<UPageWidget>> CollectiblesClasses;

	void ResetAllButtons();

	void SetCurrentButton(UBookButton* _button);

	UBookButton* GetCollectibleButton(int i);

	//Logbook
	void LB_CreatePages();
	void LB_CreateCollectibles();
	bool CheckButtonBoundaries(UBookButton* _button, FVector2D& mousePos);

public:
	virtual void Init(ABookActorTextures* _book, UBookPageWidget* _aux = nullptr);

	void InitializeElementsVisibility();

	void InitializeButtons();
	void ToggleReadMode();
	virtual void OpenBookmark(LG_Bookmarks _bookmark);
	void MoveBookMouse(FVector2D _porcentage);
	void LB_SetPage(int _page);
	void LB_SetCollectibles(int _page, bool _taleSelectedButton = false);
	void LB_SetAllCollectiblesMenuVisibility(bool _visibility);

	void NavigatePageButtons(FVector2D _value);
	void PressCurrentButton();
	void ReleaseButton();

	void ViusallyPressButton();
	void VisuallyRealeaseButton();

	//LogBook
	void LB_UnlockPage(int _pageNumber);
	UFUNCTION(BlueprintCallable)
	void LB_UnlockPage(TSubclassOf<UPageWidget> _pageToUnlock);
	void LB_UnlockAllPages();
	void LB_LockPage(TSubclassOf<UPageWidget> _pageToUnlock);
	void LB_LockAllPages();
	void LB_UnlockCollectible(TSubclassOf<UPageWidget> _pageToUnlock);
	void LB_UnlockAllCollectibles();
	void LB_LockCollectible(TSubclassOf<UPageWidget>& _pageToUnlock);
	void LB_LockAllCollectibles();
	void EnableCollectibleButton(int i, bool _enable = true);
	//void LB_UnlockCollectible(TSubclassOf<UPageWidget> _pageToUnlock);
	UFUNCTION(BlueprintCallable)
	void LB_UnlockGlyph(EGlyphs _glyphToUnlock);
	void LB_LockGlyph(EGlyphs _glyphToUnlock);
	void LB_LockAllGlyphs();
};