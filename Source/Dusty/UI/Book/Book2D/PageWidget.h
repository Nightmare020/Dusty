// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PageWidget.generated.h"

class UCanvasPanel;
class UTextBlock;

/**
 * 
 */
UCLASS()
class DUSTY_API UPageWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, category = "Page", meta = (DisplayName = "IsLocked"))
		bool m_isLocked = true;

	int m_pageNumber = 0;
	TSubclassOf<UPageWidget> m_pageClass;

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UCanvasPanel* Unlocked_Panel;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UCanvasPanel* Locked_Panel;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* PageNumber_Left;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* PageNumber_Right;

	void PageInit();
	void Unlock();
	void Lock();
	bool GetIsLocked();

	void SetPageNumber(int _value);
	int GetPageNumber();

	//Each page knows what type of page is
	TSubclassOf<UPageWidget> GetPageClass();
};
