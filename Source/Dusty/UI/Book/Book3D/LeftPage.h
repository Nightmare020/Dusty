// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BookPageWidget.h"
#include "LeftPage.generated.h"

/**
 * 
 */
UCLASS()
class DUSTY_API ULeftPage : public UBookPageWidget
{
	GENERATED_BODY()
	
protected:
#pragma region UI_Buttons_Variables
	//Logbook Buttons
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UBookButton* PreviousPage_Button;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UBookButton* GoPause_Button;
#pragma endregion


#pragma region UI_Buttons_Functions
	UFUNCTION()
		void B_PreviousPage();
	UFUNCTION()
		void B_GoPause();
#pragma endregion

public:
	//virtual void Init(ABookActorTextures* _book) override;
};
