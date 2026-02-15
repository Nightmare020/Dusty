// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Delegates/Delegate.h"
#include "Blueprint/UserWidget.h"
#include "BookButton.generated.h"


class UImage;
class UNamedSlot;
class UTextBlock;

struct FLinearColor;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPressedEvent);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReleasedEvent);

UCLASS()
class DUSTY_API UBookButton : public UUserWidget
{
	GENERATED_BODY()

private:
	bool m_canBeVisible = true;
	bool m_desiredVisibility = true;

	void UpdateVisibility();

protected:
	UPROPERTY(meta = (BindWidget))
		UImage* background;	
		UPROPERTY(meta = (BindWidget))
		UNamedSlot* ChildSlot;

		//Esto no deberia de hacerlo asi, pero la vida
		UTextBlock* PossibleText = nullptr;
		UImage* PossibleImage = nullptr;
		bool triedCatchingText = false;
		FColor TextHoveredColor = FColor(0.839216f, 0.784314f, 0.792157f);
		FColor TextUnhoveredColor = FColor(0.011765f, 0.007843f, 0.f);
		FColor TextDisabledColor = FColor(0.088542f, 0.064867f, 0.017524f);

	UPROPERTY(EditAnywhere, meta = (DisplayName="Color Normal"), Category = "Appearance")
		FLinearColor m_clrNormal;

	UPROPERTY(EditAnywhere, meta = (DisplayName="Color Highlight"), Category = "Appearance")
		FLinearColor m_clrHighlighted;

	UPROPERTY(EditAnywhere, meta = (DisplayName="Color Pressed"), Category = "Appearance")
		FLinearColor m_clrPressed;

	UPROPERTY(EditAnywhere, meta = (DisplayName="Color Disabled"), Category = "Appearance")
		FLinearColor m_clrDisabled;

	bool m_isDisabled = false;
	bool m_isPressed = false;

	USoundBase* m_buttonSound = LoadObject<USoundBase>(nullptr, TEXT("/Game/Audio/Tracks/Book/Seleccionar.Seleccionar"));


public:
	bool IsBookButtonVisible();

	//Conections
	UPROPERTY(EditAnywhere, category = "Conections")
		UBookButton* UpButton;

	UPROPERTY(EditAnywhere, category = "Conections")
		UBookButton* RightButton;

	UPROPERTY(EditAnywhere, category = "Conections")
		UBookButton* DownButton;

	UPROPERTY(EditAnywhere, category = "Conections")
		UBookButton* LeftButton;

	//CALLBACLS
	/** Called when the button is pressed */
	UPROPERTY(BlueprintAssignable, Category = "Event")
		FOnPressedEvent OnPressed;

	/** Called when the button is released */
	UPROPERTY(BlueprintAssignable, Category = "Event")
		FOnReleasedEvent OnReleased;

	void TryCatchingVisualResources();

	void Hover();
	void Unhover();
	void Pressed();
	void Released();

	void SetPressedColor();
	void SetHighlightedColor();

	bool GetIsDisabled() { return m_isDisabled; }
	void SetDisabled(bool _disabled);
	void SetDesiredVisibility(bool _value);
	void SetRealVisibility(bool _value);

	bool GetDesiredVisibility() { return m_desiredVisibility; }
};
