#include "BookActorTextures.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "Components\Button.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"
#include "Dusty/Utils/DustyUtils.h"
#include "Dusty/Character/DustyCharacter.h"
#include "Dusty/Character/CharacterStates/ActionsFilter.h"
#include "BookRenders.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "Dusty/UI/HUD/HUDSubsystem.h"
#include "Dusty/Audio/AudioSubsytem.h"

//Pages
#include "BookPageWidget.h"
#include "Dusty/UI/Book/ChangePageEndNotify.h"
#include "Dusty/UI/Book/ChangeBookmarkEndNotify.h"
#include "Dusty/UI/Book/CloseBookEndNotify.h"

//For finding the mouse
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"


ABookActorTextures::ABookActorTextures()
{
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(root);

	m_bookHardback = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Book"));
	m_bookHardback->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	m_bookmarkGlyphs = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GlyphsBookmark"));
	m_bookmarkGlyphs->AttachToComponent(m_bookHardback, FAttachmentTransformRules::KeepRelativeTransform);
	m_bookmarkLogbook = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LogbookBookmark"));
	m_bookmarkLogbook->AttachToComponent(m_bookHardback, FAttachmentTransformRules::KeepRelativeTransform);
	m_bookmarkCollectibles = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CollectiblesBookmark"));
	m_bookmarkCollectibles->AttachToComponent(m_bookHardback, FAttachmentTransformRules::KeepRelativeTransform);
	m_bookmarkSettings = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SettingsBookmark"));
	m_bookmarkSettings->AttachToComponent(m_bookHardback, FAttachmentTransformRules::KeepRelativeTransform);

	m_topLeftRef = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Top Left Reference"));
	m_topLeftRef->AttachToComponent(m_bookHardback, FAttachmentTransformRules::KeepRelativeTransform);
	m_bottomRightRef = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bottom Right Reference"));
	m_bottomRightRef->AttachToComponent(m_bookHardback, FAttachmentTransformRules::KeepRelativeTransform);
}

void ABookActorTextures::ToggleReadMode()
{
	if (m_CanOpenReadMode)
	{
		m_bookMain->ToggleReadMode();
	}
}

void ABookActorTextures::EnableReadMode(bool Enabled)
{
	m_CanOpenReadMode = Enabled;

	if (Enabled)
	{
		DustyUtils::ShowHud(GetWorld(), EHudCanvas::ReadModeButton);
	}
	else
	{
		DustyUtils::HideHud(GetWorld(), EHudCanvas::ReadModeButton);
	}
}

void ABookActorTextures::BeginPlay()
{
	Super::BeginPlay();
	m_dusty = DustyUtils::GetDustyCharacter();
	m_animInstance = m_bookHardback->GetAnimInstance();
	m_bookHardback->SetMorphTarget(TEXT("Book_Deform_Down"), 1);

	m_bookmarkGlyphs->OnClicked.AddDynamic(this, &ABookActorTextures::GlyphsBookmark_Button);
	m_bookmarkLogbook->OnClicked.AddDynamic(this, &ABookActorTextures::LogbookBookmark_Button);
	m_bookmarkCollectibles->OnClicked.AddDynamic(this, &ABookActorTextures::CollectiblesBookmark_Button);
	m_bookmarkSettings->OnClicked.AddDynamic(this, &ABookActorTextures::SettingsBookmark_Button);

	m_originBookmarkPos = m_bookmarkLogbook->GetRelativeLocation().Y;
	m_upBookmarkPos = m_bookmarkLogbook->GetRelativeLocation().Y + m_bookmarksDisplacement;

	SetUpNotificationsEvents();
	SetBookVisibility(false);
}

void ABookActorTextures::FindMousePosition()
{
	//Get Mouse Position on Screen
	float mouseX;
	float mouseY;

	if (PlayerController == nullptr)
	{
		PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	}

	if (PlayerController)
	{
		float DeltaTime = GetWorld()->GetDeltaSeconds();
		PlayerController->GetMousePosition(mouseX, mouseY);

		FVector TopLeft = m_topLeftRef->GetComponentLocation();
		FVector BottomRight = m_bottomRightRef->GetComponentLocation();
		FVector2D TopLeft_Screen;
		FVector2D BottomRight_Screen;

		PlayerController->ProjectWorldLocationToScreen(TopLeft, TopLeft_Screen, true);
		PlayerController->ProjectWorldLocationToScreen(BottomRight, BottomRight_Screen, true);

		if (mouseX > TopLeft_Screen.X && mouseX < BottomRight_Screen.X && mouseY > TopLeft_Screen.Y && mouseY < BottomRight_Screen.Y)
		{
			float porcentageX = (mouseX - TopLeft_Screen.X) / (BottomRight_Screen.X - TopLeft_Screen.X);
			float porcentageY = (mouseY - TopLeft_Screen.Y) / (BottomRight_Screen.Y - TopLeft_Screen.Y);
			FVector2D porcentage(porcentageX, porcentageY);

			m_bookVisible->MoveBookMouse(porcentage);
			//m_bookMain->MoveBookMouse(porcentage);
			//m_bookAux->MoveBookMouse(porcentage);
		}
	}
}

void ABookActorTextures::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DustyUtils::PrintMessage("open", m_isOpen, FColor::Blue, DeltaTime);
	if (m_isOpen)
	{
		ECommonInputType InputType = DustyUtils::GetCurrentInputType();

		// Check if the input is Keyboard/Mouse or Gamepad
		if (InputType == ECommonInputType::MouseAndKeyboard)
		{
			if (lookForMouse)
			{
				FindMousePosition();
			}
			if (PlayerController && m_gameInputOnly)
			{
				PlayerController->SetInputMode(FInputModeGameAndUI());
				m_gameInputOnly = false;
			}
		}
		else
		{
			if (PlayerController && !m_gameInputOnly)
			{
				PlayerController->SetInputMode(FInputModeGameOnly());
				m_gameInputOnly = true;
			}			
		}
		MoveBookmarksToPosition(DeltaTime);
	}

	if (m_glyphOpenTime > 0)
	{
		m_glyphOpenTime -= DeltaTime;
	}
	if (m_collectibleOpenTime > 0)
	{
		m_collectibleOpenTime -= DeltaTime;
	}
}

bool ABookActorTextures::NotAnimating()
{
	return !m_onAnimation;
}

void ABookActorTextures::OpenBook(LG_Bookmarks _bookmark)
{
	if (m_renders)
	{
		m_renders->RenderOpenBook();
	}

	lookForMouse = true;
	m_isOpen = true;
	SetBookVisibility(true);
	m_currentBookmark = _bookmark;
	//m_animInstance->Montage_Play(m_openMontage);

	if (m_currentBookmark == LG_Bookmarks::LogBook)
	{
		EnableReadMode(true);
		m_currentPage = m_maxPageNumberUnlocked;

		if (m_dusty->FirstTimeOpeningBook)
		{
			m_currentPage = 0;
		}
	}
	else if (m_currentBookmark == LG_Bookmarks::Collectibles)
	{
		m_bookMain->LB_SetCollectibles(m_collectibleLastOneCollected);
		m_bookAux->LB_SetCollectibles(m_collectibleLastOneCollected);
	}

	if (_bookmark == LG_Bookmarks::LogBook)
	{

	}

	m_bookMain->OpenBookmark(m_currentBookmark);
	m_bookAux->OpenBookmark(m_currentBookmark);
	MoveBookmarkToUpPos(m_currentBookmark);
}

void ABookActorTextures::CloseBook()
{
	m_onAnimation = true;
	m_isOpen = false;
	//m_animInstance->Montage_Play(m_closeMontage);
	CloseBook_End();
}

bool ABookActorTextures::ToggleBook()
{
	if (m_isOpen)
	{
		CloseBook();
		return false;
	}
	else
	{
		if (m_dusty->GetIsInSettingsMainMenu())
		{
			OpeningBookmark = LG_Bookmarks::Settings;
		} 
		else if (m_dusty->FirstTimeOpeningBook)
		{
			OpeningBookmark = LG_Bookmarks::LogBook;
		}
		else if (m_dusty->GetIsInFinalQuestion())
		{
			OpeningBookmark = LG_Bookmarks::FinalQuestion;
			EnableReadMode(true);
		}
		else if (m_collectibleOpenTime > 0)
		{
			m_collectibleOpenTime = 0;

			OpeningBookmark = LG_Bookmarks::Collectibles;
		}
		else
		{
			OpeningBookmark = LG_Bookmarks::LogBook;
		}

		OpenBook(OpeningBookmark);
		return true;
	}
}

void ABookActorTextures::SetBookResources(UBookPageWidget* _bookMain, UBookPageWidget* _bookAux, ABookRenders* _render)
{
	m_bookMain = _bookMain;
	m_bookAux = _bookAux;
	m_bookMain->Init(this, m_bookAux);
	m_bookAux->Init(this);
	m_bookVisible = m_bookMain;

	m_renders = _render;
}

void ABookActorTextures::PressBookButton()
{
	m_bookMain->PressCurrentButton();
	m_bookAux->ViusallyPressButton();
}

void ABookActorTextures::ReleaseBookButton()
{
	m_bookVisible->ReleaseButton();
	//m_bookMain->ReleaseButton();
	//m_bookAux->VisuallyRealeaseButton();
}

void ABookActorTextures::NavigateButtons(FVector2D _value)
{
	m_bookVisible->NavigatePageButtons(_value);
	//m_bookMain->NavigatePageButtons(_value);
	//m_bookAux->NavigatePageButtons(_value);
}

#pragma region LogBook Functions
void ABookActorTextures::LB_NextPage_Start()
{
	if (!m_onAnimation)
	{
		int nextpage = m_currentPage + 1;
		if (nextpage <= m_maxPageNumberUnlocked)
		{
			++m_currentPage;
			m_bookMain->LB_SetPage(m_currentPage);
			m_renders->ActivateAuxiliarRender(true);

			//playMontage NextPage
			m_onAnimation = true;
			m_animInstance->Montage_Play(m_pageNextMontage);
			m_bookVisible = m_bookMain;
			GetWorld()->GetTimerManager().SetTimer(m_PassPageHandle, this, &ABookActorTextures::LB_ChangePage_End, m_PassPageCooldown, false);

			if(m_pageSound)
			{
				GetGameInstance()->GetSubsystem<UAudioSubsytem>()->PlaySound2D(m_pageSound);
			}
			
		}
	}
}

void ABookActorTextures::LB_PreviousPage_Start()
{
	if (!m_onAnimation)
	{
		int nextpage = m_currentPage - 1;
		if (nextpage >= 0)
		{
			m_bookMain->LB_SetPage(m_currentPage);
			--m_currentPage;
			m_bookAux->LB_SetPage(m_currentPage);
			m_renders->ActivateAuxiliarRender(true);

			//playMontage PreviousPage
			m_onAnimation = true;
			m_animInstance->Montage_Play(m_pagePrevMontage);
			m_bookVisible = m_bookAux;
			GetWorld()->GetTimerManager().SetTimer(m_PassPageHandle, this, &ABookActorTextures::LB_ChangePage_End, m_PassPageCooldown, false);

			if(m_pageSound)
			{
				GetGameInstance()->GetSubsystem<UAudioSubsytem>()->PlaySound2D(m_pageSound);
			}
		}
	}
}

void ABookActorTextures::LB_GoToCollectible(int _pageNumber)
{
	if (!m_onAnimation && !m_dusty->GetIsInSettingsMainMenu() && !m_dusty->GetIsInFinalQuestion())
	{
		m_renders->ActivateAuxiliarRender(true);
		m_currentCollectiblePage = _pageNumber;
		m_bookMain->LB_SetCollectibles(m_currentCollectiblePage, true);

		//playMontage NextPage
		m_onAnimation = true;
		m_animInstance->Montage_Play(m_pageNextMontage);
		m_bookVisible = m_bookMain;
		GetWorld()->GetTimerManager().SetTimer(m_PassPageHandle, this, &ABookActorTextures::LB_ChangePage_End, m_PassPageCooldown, false);
	}
}

void ABookActorTextures::LB_ReturnToCollectiblesMenu()
{
	if (!m_onAnimation && !m_dusty->GetIsInSettingsMainMenu() && !m_dusty->GetIsInFinalQuestion())
	{
		m_bookMain->LB_SetCollectibles(m_currentCollectiblePage);
		m_currentCollectiblePage = 0;
		m_bookAux->LB_SetCollectibles(m_currentCollectiblePage);
		m_renders->ActivateAuxiliarRender(true);

		//playMontage NextPage
		m_onAnimation = true;
		m_animInstance->Montage_Play(m_pagePrevMontage);
		m_bookVisible = m_bookAux;
		GetWorld()->GetTimerManager().SetTimer(m_PassPageHandle, this, &ABookActorTextures::LB_ChangePage_End, m_PassPageCooldown, false);
	}
}

void ABookActorTextures::NextBookmark()
{
	if (!m_onAnimation && !m_dusty->GetIsInSettingsMainMenu() && !m_dusty->GetIsInFinalQuestion())
	{
		EnableReadMode(false);
		if (m_currentBookmark == LG_Bookmarks::LogBook)
		{
			OpenBookmarkWithAnimation(LG_Bookmarks::Glyphs);
		}
		else if (m_currentBookmark == LG_Bookmarks::Glyphs)
		{
			OpenBookmarkWithAnimation(LG_Bookmarks::Collectibles);
		}
		else if (m_currentBookmark == LG_Bookmarks::Collectibles)
		{
			OpenBookmarkWithAnimation(LG_Bookmarks::Settings);
		}
		else if (m_currentBookmark == LG_Bookmarks::Settings)
		{
			OpenBookmarkWithAnimation(LG_Bookmarks::LogBook);
			EnableReadMode(true);
		}

		if(m_bookmarkSound)
		{
			GetGameInstance()->GetSubsystem<UAudioSubsytem>()->PlaySound2D(m_bookmarkSound);
		}
	}
}

void ABookActorTextures::PreviousBookmark()
{
	if (!m_onAnimation && !m_dusty->GetIsInSettingsMainMenu() && !m_dusty->GetIsInFinalQuestion())
	{
		EnableReadMode(false);
		if (m_currentBookmark == LG_Bookmarks::LogBook)
		{
			OpenBookmarkWithAnimation(LG_Bookmarks::Settings);
		}
		else if (m_currentBookmark == LG_Bookmarks::Glyphs)
		{
			OpenBookmarkWithAnimation(LG_Bookmarks::LogBook);
			EnableReadMode(true);
		}
		else if (m_currentBookmark == LG_Bookmarks::Collectibles)
		{
			OpenBookmarkWithAnimation(LG_Bookmarks::Glyphs);
		}
		else if (m_currentBookmark == LG_Bookmarks::Settings)
		{
			OpenBookmarkWithAnimation(LG_Bookmarks::Collectibles);
		}

		if(m_bookmarkSound)
		{
			GetGameInstance()->GetSubsystem<UAudioSubsytem>()->PlaySound2D(m_bookmarkSound);
		}
	}
}

void ABookActorTextures::OpenBookmark(LG_Bookmarks _bookmarks)
{
	MoveBookmarkToOriginPos(m_currentBookmark);
	m_currentBookmark = _bookmarks;
	OpenBook(_bookmarks);
}

void ABookActorTextures::OpenBookmarkWithAnimation(LG_Bookmarks _bookmarks)
{
	if (_bookmarks != m_currentBookmark && !m_onAnimation)
	{
		//Next Bookmark
		if (_bookmarks > m_currentBookmark)
		{
			m_renders->ActivateAuxiliarRender(true);
			MoveBookmarkToOriginPos(m_currentBookmark);
			m_currentBookmark = _bookmarks;
			MoveBookmarkToUpPos(m_currentBookmark);

			m_bookMain->OpenBookmark(m_currentBookmark);

			//playMontage nextBookmark
			m_onAnimation = true;
			m_animInstance->Montage_Play(m_bookmarkNextMontage);
			m_bookVisible = m_bookMain;
			GetWorld()->GetTimerManager().SetTimer(m_PassBookmarkHandle, this, &ABookActorTextures::ChangeBookmark_End, m_PassBookmarkCooldown, false);
		}
		//Previous Bookmark
		else
		{
			m_renders->ActivateAuxiliarRender(true);
			//m_bookMain->OpenBookmark(m_currentBookmark);

			MoveBookmarkToOriginPos(m_currentBookmark);
			m_currentBookmark = _bookmarks;
			MoveBookmarkToUpPos(m_currentBookmark);

			m_bookAux->OpenBookmark(m_currentBookmark);
			//playMontage previousBookmark
			m_onAnimation = true;
			m_animInstance->Montage_Play(m_bookmarkPrevMontage);
			m_bookVisible = m_bookAux;
			GetWorld()->GetTimerManager().SetTimer(m_PassBookmarkHandle, this, &ABookActorTextures::ChangeBookmark_End, m_PassBookmarkCooldown, false);
		}
	}
}

void ABookActorTextures::GoBack()
{
	CloseBook();
}
#pragma endregion

#pragma region Unlock Functions
void ABookActorTextures::LB_UnlockPage(int _pageNumber)
{
	if (m_bookMain)
	{
		m_bookMain->LB_UnlockPage(_pageNumber);
	}
}

void ABookActorTextures::LB_UnlockPage(TSubclassOf<UPageWidget> _pageToUnlock, bool _UnlockOnLoad)
{
	if (m_bookMain && m_bookAux)
	{
		if (!_UnlockOnLoad)
		{
			DustyUtils::ShowHud(GetWorld(), EHudCanvas::DiaryNotification, 0);
			UAudioSubsytem* audioSubsystem = GetGameInstance()->GetSubsystem<UAudioSubsytem>();
			audioSubsystem->PlaySound2D(m_sound);
		}

		m_bookMain->LB_UnlockPage(_pageToUnlock);
		m_bookAux->LB_UnlockPage(_pageToUnlock);
	}
}

void ABookActorTextures::LB_UnlockAllPages()
{
	if (m_bookMain && m_bookAux)
	{
		m_bookMain->LB_UnlockAllPages();
		m_bookAux->LB_UnlockAllPages();
	}
}

void ABookActorTextures::LB_UnlockAllCollectibles()
{
	if (m_bookMain && m_bookAux)
	{
		m_bookMain->LB_UnlockAllCollectibles();
		m_bookAux->LB_UnlockAllCollectibles();
	}
}

void ABookActorTextures::LB_LockPage(TSubclassOf<UPageWidget> _pageToUnlock)
{
	if (m_bookMain && m_bookAux)
	{
		m_bookMain->LB_LockPage(_pageToUnlock);
		m_bookAux->LB_LockPage(_pageToUnlock);
	}
}

void ABookActorTextures::LB_RestartBookProgress()
{
	if (m_bookMain && m_bookAux)
	{
		m_collectibleOpenTime = 0;
		m_bookMain->LB_LockAllPages();
		m_bookAux->LB_LockAllPages();
		m_bookMain->LB_LockAllCollectibles();
		m_bookAux->LB_LockAllCollectibles();
		m_bookMain->LB_LockAllGlyphs();
		m_bookAux->LB_LockAllGlyphs();
	}
}

void ABookActorTextures::LB_UnlockCollectible(TSubclassOf<UPageWidget> _collectibleToUnlock, int _collectiblePageNumber, bool _UnlockOnLoad)
{
	if (m_bookMain)
	{
		if (!_UnlockOnLoad)
		{
			m_collectibleOpenTime = m_generalOpenMaxTime;
			m_collectibleLastOneCollected = _collectiblePageNumber;
		}
		m_bookMain->LB_UnlockCollectible(_collectibleToUnlock);
		m_bookAux->LB_UnlockCollectible(_collectibleToUnlock);

		//We force one capture to ensure the Aux is updated
		m_renders->ActivateAuxiliarRender(true);
		m_renders->ActivateAuxiliarRender(false);
	}
}

void ABookActorTextures::LB_UnlockGlyph(EGlyphs _glyphToUnlock, TSubclassOf<UPageWidget> _pageToUnlock, bool _UnlockOnLoad)
{
	if (m_bookMain)
	{
		if (!_UnlockOnLoad)
		{
			m_glyphOpenTime = m_generalOpenMaxTime;
			DustyUtils::ShowHud(GetWorld(), EHudCanvas::DiaryNotification, 2);
		}

		m_bookMain->LB_UnlockPage(_pageToUnlock);
		m_bookAux->LB_UnlockPage(_pageToUnlock);
		m_bookMain->LB_UnlockGlyph(_glyphToUnlock);
		m_bookAux->LB_UnlockGlyph(_glyphToUnlock);
	}
}
#pragma endregion

#pragma region Notifies Functions
void ABookActorTextures::SetUpNotificationsEvents()
{
	const TArray<FAnimNotifyEvent> pageNotifies = m_pageNextMontage->Notifies;

	for (FAnimNotifyEvent eventNotify : pageNotifies)
	{
		if (UChangePageEndNotify* ChangePageNotify = Cast<UChangePageEndNotify>(eventNotify.Notify))
		{
			ChangePageNotify->OnNotified.AddUObject(this, &ABookActorTextures::LB_ChangePage_End);
		}
	}
	const TArray<FAnimNotifyEvent> pagePrevNotifies = m_pagePrevMontage->Notifies;

	for (FAnimNotifyEvent eventNotify : pagePrevNotifies)
	{
		if (UChangePageEndNotify* ChangePageNotify = Cast<UChangePageEndNotify>(eventNotify.Notify))
		{
			ChangePageNotify->OnNotified.AddUObject(this, &ABookActorTextures::LB_ChangePage_End);
		}
	}

	const TArray<FAnimNotifyEvent> bookmarkNotifies = m_bookmarkNextMontage->Notifies;

	for (FAnimNotifyEvent eventNotify : bookmarkNotifies)
	{
		if (UChangeBookmarkEndNotify* ChangeBookmarkNotify = Cast<UChangeBookmarkEndNotify>(eventNotify.Notify))
		{
			ChangeBookmarkNotify->OnNotified.BindUObject(this, &ABookActorTextures::ChangeBookmark_End);
		}
	}

	const TArray<FAnimNotifyEvent> bookmarkPrevNotifies = m_bookmarkPrevMontage->Notifies;

	for (FAnimNotifyEvent eventNotify : bookmarkPrevNotifies)
	{
		if (UChangeBookmarkEndNotify* ChangeBookmarkNotify = Cast<UChangeBookmarkEndNotify>(eventNotify.Notify))
		{
			ChangeBookmarkNotify->OnNotified.BindUObject(this, &ABookActorTextures::ChangeBookmark_End);
		}
	}

	const TArray<FAnimNotifyEvent> closeBookNotifies = m_closeMontage->Notifies;

	for (FAnimNotifyEvent eventNotify : closeBookNotifies)
	{
		if (UCloseBookEndNotify* CloseBookNotify = Cast<UCloseBookEndNotify>(eventNotify.Notify))
		{
			CloseBookNotify->OnClosedBook.AddUObject(this, &ABookActorTextures::CloseBook_End);
		}
	}
}

void ABookActorTextures::LB_ChangePage_End()
{
	if (m_currentBookmark == LG_Bookmarks::LogBook)
	{
		m_bookMain->LB_SetPage(m_currentPage);
		m_bookAux->LB_SetPage(m_currentPage);
	}
	else if(m_currentBookmark == LG_Bookmarks::Collectibles)
	{
		m_bookMain->LB_SetCollectibles(m_currentCollectiblePage);
		m_bookAux->LB_SetCollectibles(m_currentCollectiblePage, true);
	}

	m_renders->ActivateAuxiliarRender(false);
	m_onAnimation = false;
}

void ABookActorTextures::ChangeBookmark_End()
{
	if (m_bookMain != nullptr)
	{
		m_bookMain->OpenBookmark(m_currentBookmark);
		m_bookAux->OpenBookmark(m_currentBookmark);
		m_onAnimation = false;
		m_renders->ActivateAuxiliarRender(false);
	}
}

void ABookActorTextures::CloseBook_End()
{
	if (m_renders)
	{
		m_renders->RenderCloseBook();
	}

	MoveBookmarkToOriginPos(m_currentBookmark);

	m_isOpen = false;
	//SetActorTickEnabled(false);
	SetBookVisibility(false);
	m_onAnimation = false;

	OpeningBookmark = LG_Bookmarks::LogBook;
}

void ABookActorTextures::LogbookBookmark_Button(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed)
{
	if (!m_dusty->GetIsDustyInFinalQuestion() && !m_dusty->FirstTimeOpeningBook)
	{
		DustyUtils::HideHudWithNoFade(GetWorld(), EHudCanvas::ReadMode);
		DustyUtils::HideHudWithNoFade(GetWorld(), EHudCanvas::ReadModeButton);
		OpenBookmarkWithAnimation(LG_Bookmarks::LogBook);
	}
}
void ABookActorTextures::GlyphsBookmark_Button(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed)
{
	if (!m_dusty->GetIsDustyInFinalQuestion() && !m_dusty->FirstTimeOpeningBook)
	{
		DustyUtils::HideHudWithNoFade(GetWorld(), EHudCanvas::ReadMode);
		DustyUtils::HideHudWithNoFade(GetWorld(), EHudCanvas::ReadModeButton);
		OpenBookmarkWithAnimation(LG_Bookmarks::Glyphs);
	}
}
void ABookActorTextures::CollectiblesBookmark_Button(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed)
{
	if (!m_dusty->GetIsDustyInFinalQuestion() && !m_dusty->FirstTimeOpeningBook)
	{
		DustyUtils::HideHudWithNoFade(GetWorld(), EHudCanvas::ReadMode);
		DustyUtils::HideHudWithNoFade(GetWorld(), EHudCanvas::ReadModeButton);
		OpenBookmarkWithAnimation(LG_Bookmarks::Collectibles);
	}
}
void ABookActorTextures::SettingsBookmark_Button(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed)
{
	if (!m_dusty->GetIsDustyInFinalQuestion() && !m_dusty->FirstTimeOpeningBook)
	{
		DustyUtils::HideHudWithNoFade(GetWorld(), EHudCanvas::ReadMode);
		DustyUtils::HideHudWithNoFade(GetWorld(), EHudCanvas::ReadModeButton);
		OpenBookmarkWithAnimation(LG_Bookmarks::Settings);
	}
}
#pragma endregion

#pragma region Bookmark Move Functions
void ABookActorTextures::MoveBookmarksToPosition(float DeltaTime)
{
	if (m_bookmarkToOriginPos)
	{
		FVector originalPos = FVector(m_bookmarkToOriginPos->GetRelativeLocation().X, m_originBookmarkPos, m_bookmarkToOriginPos->GetRelativeLocation().Z);
		FVector NewPos = FMath::Lerp(m_bookmarkToOriginPos->GetRelativeLocation(), originalPos, DeltaTime * m_bookmarkSpeed);
		float distance = (originalPos - NewPos).GetAbs().Size();
		if (distance > 0.04f)
		{
			m_bookmarkToOriginPos->SetRelativeLocation(NewPos);
		}
		else
		{
			m_bookmarkToOriginPos->SetRelativeLocation(originalPos);
			m_bookmarkToOriginPos = nullptr;
		}
	}
	if (m_bookmarkToUpPos)
	{
		FVector UpPos = FVector(m_bookmarkToUpPos->GetRelativeLocation().X, m_upBookmarkPos, m_bookmarkToUpPos->GetRelativeLocation().Z);
		FVector NewPos = FMath::Lerp(m_bookmarkToUpPos->GetRelativeLocation(), UpPos, DeltaTime * m_bookmarkSpeed);
		float distance = (UpPos - NewPos).GetAbs().Size();
		if (distance > 0.04f)
		{
			m_bookmarkToUpPos->SetRelativeLocation(NewPos);
		}
		else
		{
			m_bookmarkToUpPos->SetRelativeLocation(UpPos);
			m_bookmarkToUpPos = nullptr;
		}
	}
}

void ABookActorTextures::MoveBookmarkToUpPos(LG_Bookmarks _bookmark)
{
	if (_bookmark == LG_Bookmarks::LogBook)
	{
		MoveBookmarkToUpPos(m_bookmarkLogbook);
	}
	else if (_bookmark == LG_Bookmarks::Glyphs)
	{
		MoveBookmarkToUpPos(m_bookmarkGlyphs);
	}
	else if (_bookmark == LG_Bookmarks::Collectibles)
	{
		MoveBookmarkToUpPos(m_bookmarkCollectibles);
	}
	else if (_bookmark == LG_Bookmarks::Settings)
	{
		MoveBookmarkToUpPos(m_bookmarkSettings);
	}
}

void ABookActorTextures::MoveBookmarkToUpPos(UStaticMeshComponent* _bookmark)
{
	if (m_bookmarkToUpPos)
	{
		FVector UpPos = FVector(m_bookmarkToUpPos->GetRelativeLocation().X, m_upBookmarkPos, m_bookmarkToUpPos->GetRelativeLocation().Z);
		m_bookmarkToUpPos->SetRelativeLocation(UpPos);
	}
	m_bookmarkToUpPos = _bookmark;
}

void ABookActorTextures::MoveBookmarkToOriginPos(LG_Bookmarks _bookmark)
{
	if (_bookmark == LG_Bookmarks::LogBook)
	{
		MoveBookmarkToOriginPos(m_bookmarkLogbook);
	}
	else if (_bookmark == LG_Bookmarks::Glyphs)
	{
		MoveBookmarkToOriginPos(m_bookmarkGlyphs);
	}
	else if (_bookmark == LG_Bookmarks::Collectibles)
	{
		MoveBookmarkToOriginPos(m_bookmarkCollectibles);
	}
	else if (_bookmark == LG_Bookmarks::Settings)
	{
		MoveBookmarkToOriginPos(m_bookmarkSettings);
	}
}

void ABookActorTextures::MoveBookmarkToOriginPos(UStaticMeshComponent* _bookmark)
{
	if (m_bookmarkToOriginPos)
	{
		FVector originalPos = FVector(m_bookmarkToOriginPos->GetRelativeLocation().X, m_originBookmarkPos, m_bookmarkToOriginPos->GetRelativeLocation().Z);
		m_bookmarkToOriginPos->SetRelativeLocation(originalPos);
	}
	m_bookmarkToOriginPos = _bookmark;
}
#pragma endregion

void ABookActorTextures::SetBookVisibility(bool _value)
{
	if (_value)
	{
		SetActorTickEnabled(true);

		if (PlayerController == nullptr)
		{
			PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		}
		if (PlayerController)
		{
			PlayerController->SetInputMode(FInputModeGameAndUI());
			m_gameInputOnly = false;
			PlayerController->bShowMouseCursor = true;
		}

		if (!m_bookHardback->GetVisibleFlag())
		{
			m_bookHardback->SetVisibility(true, true);
		}
	}
	else
	{
		if (PlayerController == nullptr)
		{
			PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		}
		if (PlayerController)
		{
			if (m_dusty->ActionsFilterComponent->GetCurrentBaseStateClass() != UMainMenuState::StaticClass())
			{
				PlayerController->SetInputMode(FInputModeGameOnly());
				m_gameInputOnly = true;
				PlayerController->bShowMouseCursor = false;
			}
		}

		if (m_bookHardback->GetVisibleFlag())
		{
			m_bookHardback->SetVisibility(false, true);
		}
	}

}