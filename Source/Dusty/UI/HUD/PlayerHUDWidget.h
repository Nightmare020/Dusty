// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUDWidget.generated.h"

/**
 * 
 */

 // Forward declaration
class UTextBlock;
class UCanvasPanel;
class UImage;
class UProgressBar;
class UCommonActionWidget;

UCLASS()
class DUSTY_API UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	

	/** Check if the skip cinematic canvas is visible */
		bool IsSkipCinematicCanvasVisible();
	
	/** Make Skip Cinematic Canvas Visible */
		void SkipCinematicCanvasVisible();

	/** Make Skip Cinematic Canvas Hidden */
		void SkipCinematicCanvasHidden();

	

	/** Fade In and Out Methods */
	UFUNCTION(BlueprintCallable)
		void FadeInCanvas(UCanvasPanel* CanvasPanel, float Duration);

	UFUNCTION(BlueprintCallable)
		void FadeOutCanvas(UCanvasPanel* CanvasPanel, float Duration);

		

		UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UCanvasPanel* SkipCinematicCanvas;		

		UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UProgressBar* SkipCinematicHoldBar;


		/** Fade In Canvas Widget Time */
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float FadeInTime = 1.f;

		/** Fade In Canvas Widget Time */
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float FadeOutTime = 1.f;

private:
	bool m_IsFadingIn = false;;
	bool m_IsFadingOut = false;
	float m_FadeDuration = 0.f;
	float m_CurrentFadeTime = 0.f;
	UCanvasPanel* m_TargetCanvasPanel = nullptr;

	void UpdateFade(float DeltaTime);
};
