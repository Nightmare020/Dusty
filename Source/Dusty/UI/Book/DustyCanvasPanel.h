// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CanvasPanel.h"
#include "DustyCanvasPanel.generated.h"

class UBookButton;

UCLASS()
class DUSTY_API UDustyCanvasPanel : public UCanvasPanel
{
	GENERATED_BODY()

private:
	bool m_canBeVisible = true;
	bool m_desiredVisibility = true;

	void UpdateVisibility();
	void ShowChildren();
	void HideChildren();

public:
	UPROPERTY(EditAnywhere, category = "Conections")
	UBookButton* FirstButton = nullptr;

	void Show();
	void Hide();	

	void SetDesiredVisibility(bool _value);
};
