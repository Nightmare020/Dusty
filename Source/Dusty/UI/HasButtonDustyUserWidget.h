// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DustyUserWidget.h"
#include "CommonInputTypeEnum.h"
#include "HasButtonDustyUserWidget.generated.h"


class UPanelWidget;

UCLASS()
class DUSTY_API UHasButtonDustyUserWidget : public UDustyUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;

	virtual void SwitchInput(bool Gamepad) override;


	UPROPERTY(meta = (BindWidget))
	UPanelWidget* ButtonsBox;

	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	TArray<UTexture2D*> KeyboardButtonImages;
	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	TArray<UTexture2D*> GamepadButtonImages;
};
