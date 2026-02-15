// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../../../Utils/DustyConst.h"
#include "BookmarkWidget.generated.h"

class UButton;
class ABookActorTextures;

UCLASS()
class DUSTY_API UBookmarkWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UButton* ButtonBookmark = nullptr;

	ABookActorTextures* m_book = nullptr;
	LG_Bookmarks m_bookmarkType = LG_Bookmarks::LogBook;

	UFUNCTION()
	void B_OpenBookmark();

public:
	void InitBookmark(ABookActorTextures* _book, LG_Bookmarks _value);
};
