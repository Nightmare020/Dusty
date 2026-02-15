// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Dusty/Utils/DustyConst.h"
#include "DustyHUD.generated.h"

class UDustyUserWidget;
class UDoublePromptDustyUserWidget;
class UReadModeDustyUserWidget;
/**
 * 
 */
UCLASS()
class DUSTY_API ADustyHUD : public AHUD
{
	GENERATED_BODY()

	TMap<EHudCanvas, UDustyUserWidget*> m_WidgetsInstancesMap;

	UDoublePromptDustyUserWidget* m_DoublePromptUserWidget;
	UReadModeDustyUserWidget* m_ReadModeUserWidget;
public:

	UPROPERTY(EditDefaultsOnly, Category = "CanvasClasses")
		TMap<EHudCanvas, TSubclassOf<UDustyUserWidget>> WidgetsClassesMap;

	void ShowHud(EHudCanvas CanvasName, int id = 0);
	void ShowHud(EHudCanvas CanvasName);
	void ShowDoublePromptHud(int FirstButtonId, int SecondButtonId);
	void ShowDoublePromptHud();
	void ShowReadMode(FText Title, FText Content);
	void HideHud(EHudCanvas CanvasName);
	void HideHudWithNoFade(EHudCanvas CanvasName);
	void HideDoublePromptHud();

	void SwapReadModeTexts(FText Title, FText Content);

	bool IsHudVisible(EHudCanvas CanvasName);

	virtual void BeginPlay() override;
};
