// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HasButtonDustyUserWidget.h"
#include "PrompyButtonDustyUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class DUSTY_API UPrompyButtonDustyUserWidget : public UHasButtonDustyUserWidget
{
	GENERATED_BODY()

protected:
	virtual void SetId(int id) override;

	virtual void SwitchInput(bool Gamepad) override;
	
};
