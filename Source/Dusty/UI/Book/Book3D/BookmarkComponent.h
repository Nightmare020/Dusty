// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "../../../Utils/DustyConst.h"
#include "BookmarkComponent.generated.h"

/**
 * 
 */
UCLASS()
class DUSTY_API UBookmarkComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, category = "Bookmark", meta = (DisplayName = "Bookmark", AllowPrivateAccess = "true"))
	LG_Bookmarks m_bookmark = LG_Bookmarks::LogBook;

public:
	//virtual void BeginPlay() override;
	void Init();
};
