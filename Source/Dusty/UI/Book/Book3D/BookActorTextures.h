// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../../../Utils/DustyConst.h"
#include "BookActorTextures.generated.h"

class UBookPageWidget;
class UImage;
class UCanvasPanel;
class UButton;
class UWidgetSwitcher;
class ADustyCharacter;
class ABookRenders;
class UPageWidget;
class UAnimMontage;
class UAnimInstance;
class USoundBase;

USTRUCT()
struct FPageData
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	int number = 0;
	UPROPERTY(EditAnywhere)
	bool locked = false;
	UPROPERTY(EditAnywhere)
	UTexture* texture;
};

UCLASS()
class DUSTY_API ABookActorTextures : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, DisplayName = "Book")
	USkeletalMeshComponent* m_bookHardback = nullptr;


	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* m_bookmarkGlyphs = nullptr;
	UPROPERTY(EditAnywhere)		
		UStaticMeshComponent* m_bookmarkLogbook = nullptr;
	UPROPERTY(EditAnywhere)		
		UStaticMeshComponent* m_bookmarkCollectibles = nullptr;
	UPROPERTY(EditAnywhere)		
		UStaticMeshComponent* m_bookmarkSettings = nullptr;

	UPROPERTY(Transient)
		UStaticMeshComponent* m_bookmarkToUpPos = nullptr;
	UPROPERTY(Transient)
		UStaticMeshComponent* m_bookmarkToOriginPos = nullptr;
	float m_originBookmarkPos;
	float m_upBookmarkPos;
	float m_bookmarkSpeed = 3;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* m_topLeftRef = nullptr;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* m_bottomRightRef = nullptr;

	UPROPERTY(EditAnywhere, DisplayName = "Bookmark Selected Up Movement", category = "Book")
	float m_bookmarksDisplacement = 2.5f;
	UPROPERTY(EditAnywhere, DisplayName = "General Open Time", category = "Book")
	float m_generalOpenMaxTime = 4;
	float m_glyphOpenTime = 0;
	float m_collectibleOpenTime = 0;


#pragma region Timers
private:
	FTimerHandle m_PassPageHandle;
	
	UPROPERTY(EditDefaultsOnly)
	float m_PassPageCooldown = 0.5;

	FTimerHandle m_PassBookmarkHandle;
	
	UPROPERTY(EditDefaultsOnly)
	float m_PassBookmarkCooldown = 0.5;


#pragma endregion

# pragma region Animation Montages
	UPROPERTY(EditAnywhere, DisplayName = "AM Open", category = "Book|Animations")
		UAnimMontage* m_openMontage = nullptr;

	UPROPERTY(EditAnywhere, DisplayName = "AM Close", category = "Book|Animations")
		UAnimMontage* m_closeMontage = nullptr;

	UPROPERTY(EditAnywhere, DisplayName = "AM Page - Next", category = "Book|Animations")
		UAnimMontage* m_pageNextMontage = nullptr;

	UPROPERTY(EditAnywhere, DisplayName = "AM Page - Previus", category = "Book|Animations")
		UAnimMontage* m_pagePrevMontage = nullptr;
	
	UPROPERTY(EditAnywhere, DisplayName = "AM Bookmark - Next", category = "Book|Animations")
		UAnimMontage* m_bookmarkNextMontage = nullptr;
	
	UPROPERTY(EditAnywhere, DisplayName = "AM Bookmark - Previus", category = "Book|Animations")
		UAnimMontage* m_bookmarkPrevMontage = nullptr;
#pragma endregion

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"))
		FString m_NewEntryMessage = "New Diary Entry Added";
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"))
		FString m_NewGlyphMessage = "New Glyph Learned";
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"))
		FString m_NewCollectibleMessage = "New Collectible Added";

	UPROPERTY(Transient)
	ADustyCharacter* m_dusty = nullptr;
	UPROPERTY(Transient)
	ABookRenders* m_renders = nullptr;
	UPROPERTY(Transient)
	UAnimInstance* m_animInstance = nullptr;
	UPROPERTY(Transient)
	UBookPageWidget* m_bookMain = nullptr;
	UPROPERTY(Transient)
	UBookPageWidget* m_bookAux = nullptr;

	UPROPERTY(Transient)
	UBookPageWidget* m_bookVisible = nullptr;

	APlayerController* PlayerController = nullptr;

	int m_currentPage = 0;
	int m_maxPageNumberUnlocked = 0; // The biggest value of page number of all the UPageWidgets that are currently unlocked
	int m_currentCollectiblePage = 0;
	int m_collectibleLastOneCollected = 0;
	bool m_isOpen = false;
	bool m_onAnimation = false;
	bool m_gameInputOnly = false;
	LG_Bookmarks m_currentBookmark;


	//Finds the mousePosition over the book and gives it to the Widget
	void FindMousePosition();

public:
	bool lookForMouse = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		USoundBase* m_sound;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USoundBase* m_pageSound;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USoundBase* m_bookmarkSound;

	LG_Bookmarks OpeningBookmark = LG_Bookmarks::LogBook;
	ABookActorTextures();

	void ToggleReadMode();
	void EnableReadMode(bool Enabled);

	bool m_CanOpenReadMode = false;

	virtual void BeginPlay() override;
	void SetUpNotificationsEvents();
	virtual void Tick(float DeltaTime) override;

	void MoveBookmarksToPosition(float DeltaTime);

	bool NotAnimating();
	void OpenBook(LG_Bookmarks _bookmark);
	void CloseBook();
	bool ToggleBook();

	//This should only be called by the ABookRenders once at the begining
	void SetBookResources(UBookPageWidget* _bookMain, UBookPageWidget* _bookAux, ABookRenders* _render);

	//Setters
	void SetBookVisibility(bool _value);
	void SetMaxPageUnlocked(int _page) { m_maxPageNumberUnlocked = _page; }
	void SetCurrentCollectible(int _collectible) { m_currentCollectiblePage = _collectible; }
	void SetCurrentPage(int _page) { m_currentPage = _page; }

	//Getters
	LG_Bookmarks GetCurrentBookmark() { return m_currentBookmark; }
	int GetMaxPageUnlocked() { return m_maxPageNumberUnlocked; }
	int GetCurrentCollectible() { return m_currentCollectiblePage; }
	bool GetIsOpen() { return m_isOpen; }
	int GetCurrentPage() { return m_currentPage; }

	void PressBookButton();
	void ReleaseBookButton();
	void NavigateButtons(FVector2D _value);

	//LogBook
	void LB_UnlockPage(int _pageNumber);
	UFUNCTION(BlueprintCallable)
	void LB_UnlockPage(TSubclassOf<UPageWidget> _pageToUnlock, bool _UnlockOnLoad = false);
	UFUNCTION(BlueprintCallable)
	void LB_UnlockAllPages();
	void LB_UnlockAllCollectibles();
	UFUNCTION(BlueprintCallable)
	void LB_LockPage(TSubclassOf<UPageWidget> _pageToUnlock);
	UFUNCTION(BlueprintCallable)
	void LB_RestartBookProgress();
	UFUNCTION(BlueprintCallable)
	void LB_UnlockCollectible(TSubclassOf<UPageWidget> _collectibleToUnlock, int _collectiblePageNumber, bool _UnlockOnLoad = false);
	UFUNCTION(BlueprintCallable)
	void LB_UnlockGlyph(EGlyphs _glyphToUnlock, TSubclassOf<UPageWidget> _pageToUnlock, bool _UnlockOnLoad = false);

	void LB_NextPage_Start();
	void LB_PreviousPage_Start();

	void LB_GoToCollectible(int _pageNumber);
	void LB_ReturnToCollectiblesMenu();

	void NextBookmark();
	void PreviousBookmark();
	void OpenBookmark(LG_Bookmarks _bookmarks);
	void OpenBookmarkWithAnimation(LG_Bookmarks _bookmarks);
	void GoBack();

	//Montage Notifies
	UFUNCTION()
	void LB_ChangePage_End();
	UFUNCTION()
	void ChangeBookmark_End();
	UFUNCTION()
	void CloseBook_End();

	UFUNCTION()
		void LogbookBookmark_Button(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed);
	UFUNCTION()
		void GlyphsBookmark_Button(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed);
	UFUNCTION()
		void CollectiblesBookmark_Button(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed);
	UFUNCTION()
		void SettingsBookmark_Button(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed);

	void MoveBookmarkToUpPos(LG_Bookmarks _bookmark);
	void MoveBookmarkToUpPos(UStaticMeshComponent* _bookmark);
	void MoveBookmarkToOriginPos(LG_Bookmarks _bookmark);
	void MoveBookmarkToOriginPos(UStaticMeshComponent* _bookmark);
};
