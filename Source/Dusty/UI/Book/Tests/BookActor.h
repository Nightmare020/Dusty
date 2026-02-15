// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../../../Utils/DustyConst.h"

#include "BookActor.generated.h"

class UWidgetComponent;
class UPageWidget;
class ULeftPage;
class URightPage;
class UImage;
class UCanvasPanel;
class UButton;
class UWidgetSwitcher;

UCLASS()
class DUSTY_API ABookActor : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, DisplayName = "Book Hardback")
		UStaticMeshComponent* m_bookHardback;
	UPROPERTY(EditAnywhere, DisplayName = "Book Pages")
		UStaticMeshComponent* m_bookPages;
	UPROPERTY(EditAnywhere, DisplayName = "Single Page")
		UStaticMeshComponent* m_singlePage;

	UPROPERTY(EditAnywhere, DisplayName = "Right Page")
		UWidgetComponent* m_rightPageCmp;
	UPROPERTY(EditAnywhere, DisplayName = "Left Page")
		UWidgetComponent* m_leftPageCmp;

private:
	//TArray<UPageWidget*> pagesList;
	TArray<UPageWidget*> m_pagesLockedList;
	int m_currenPage;
	//int m_totalUnlockedPages;
	int m_maxPageNumberUnlocked; //The biggest value of page number of all the UPageWidgets that are currently unlocked
	bool m_isOpen;

	URightPage* m_rightPage;
	ULeftPage* m_leftPage;

	UPROPERTY(EditAnywhere)
	USoundBase* m_pageSound;

public:
	//All UUserWidgets added to this array should inherit from
	UPROPERTY(EditAnywhere, category = "Book")
		TArray<TSubclassOf<UPageWidget>> pagesClasses;

#pragma region UI_Buttons_Variables
	//Pause Buttons
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* Resume_Button;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* LogBook_Button;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* Settings_Button;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* Quit_Button;

	//LogBook Buttons
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* NextPage_Button;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* PreviusPage_Button;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* GoPause_Button;

#pragma endregion

	// Sets default values for this actor's properties
	ABookActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Logbook
	void LB_CreatePages();
	void LB_NextPage();
	void LB_PreviusPage();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OpenBook(LG_Bookmarks _bookmark);
	void CloseBook();
	void ToggleBook();

	//LogBook
	void LB_UnlockPage(int _pageNumber);
	UFUNCTION(BlueprintCallable)
	void LB_UnlockPage(TSubclassOf<UPageWidget> _pageToUnlock);

#pragma region UI_Buttons_Functions
	//One function per button
	UFUNCTION()
		void B_Resume();
	UFUNCTION()
		void B_Logbook();
	UFUNCTION()
		void B_Settings();
	UFUNCTION()
		void B_Quit();
	UFUNCTION()
		void B_NextPage();
	UFUNCTION()
		void B_PreviusPage();
	UFUNCTION()
		void B_GoPause();
#pragma endregion
};
