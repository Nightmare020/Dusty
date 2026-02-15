// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DustyUserWidget.h"
#include "CommonInputTypeEnum.h"
#include "DoublePromptDustyUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class DUSTY_API UDoublePromptDustyUserWidget : public UDustyUserWidget
{
	GENERATED_BODY()

private:

	int m_FirstButtonId;
	int m_SecondButtonId;

	UPROPERTY(meta = (BindWidget))
		UPanelWidget* ButtonsBox;

	UPROPERTY(EditDefaultsOnly, Category = "HUD")
		TArray<UTexture2D*> KeyboardButtonImages;
	UPROPERTY(EditDefaultsOnly, Category = "HUD")
		TArray<UTexture2D*> GamepadButtonImages;

public:
	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;
	void SetButtonsIds(int FirstButtonId, int SecondButtonId);
	void SwitchInput(bool Gamepad);
	
};
