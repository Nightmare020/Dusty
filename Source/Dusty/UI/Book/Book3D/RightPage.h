// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BookPageWidget.h"
#include "RightPage.generated.h"

UCLASS()
class DUSTY_API URightPage : public UBookPageWidget
{
	GENERATED_BODY()
	
protected:
#pragma region UI_Buttons_Variables
	//Pause Buttons
//	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
//		UBookButton* Resume_Button;
//
//	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
//		UBookButton* LogBook_Button;
//
//	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
//		UBookButton* Glyphs_Button;
//
//	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
//		UBookButton* Settings_Button;
//
//	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
//		UBookButton* Quit_Button;
//
//	//LogBook Buttons
//	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
//		UBookButton* NextPage_Button;
//#pragma endregion
//
//
//#pragma region UI_Buttons_Functions
//	//One function per button
//	UFUNCTION()
//		void B_Resume();
//	UFUNCTION()
//		void B_Logbook();
//	UFUNCTION()
//		void B_Glyphs();
//	UFUNCTION()
//		void B_Settings();
//	UFUNCTION()
//		void B_Quit();
//	UFUNCTION()
//		void B_NextPage();
#pragma endregion

public:
	//virtual void Init(ABookActorTextures* _book) override;
};
