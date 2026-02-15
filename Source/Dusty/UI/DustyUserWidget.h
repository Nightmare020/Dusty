// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DustyUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class DUSTY_API UDustyUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void Show();
	virtual void Hide();
	virtual void HideWithNoFade();
	virtual void SwitchInput(bool Gamepad) {}

	UFUNCTION(BlueprintImplementableEvent)
		void EnterAnimation();
	UFUNCTION(BlueprintImplementableEvent)
		void ExitAnimation();

	virtual void SetId(int id);

	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;

protected:

	void UpdateFade(float DeltaTime);

	int m_id = 0;

	bool m_IsFadingIn = false;;
	bool m_IsFadingOut = false;
	float m_CurrentFadeTime = 0.f;

	FTimerHandle m_ShowTimeHandle;

	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	bool m_AutoExit = false;
	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	float m_ShowTime = 4.f;
	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	float m_EnterFadeDuration = 0.5f;
	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	float m_ExitFadeDuration = 0.5f;

	
};
