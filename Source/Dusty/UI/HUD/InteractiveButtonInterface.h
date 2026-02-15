// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/MaterialBillboardComponent.h"
#include "InteractiveButtonInterface.generated.h"

/**
 *
 */

 // Forward declaration
enum class ECommonInputType : uint8;

UCLASS()
class DUSTY_API UInteractiveButtonInterface : public UMaterialBillboardComponent
{
	GENERATED_BODY()

public:
	UInteractiveButtonInterface();

	/** Update the button image depending on the input caught */
	void UpdateButtonIcon();

	// Button Icon Getters
	UTexture2D* GetkeyboardButtonIcon() { return KeyboardMouseButtonIcon; }

	void SetMaterialAlpha(float Alpha);
	float GetMaterialAlpha();
	FVector2D GetMaterialScale();
	void SetMaterialScale(FVector2D Scale);

protected:
	virtual void BeginPlay() override;

private:
	/** Keyboard button image */
	UPROPERTY(EditAnywhere, Category = "Button Icon")
		UTexture2D* KeyboardMouseButtonIcon;

	/** Gamepad button image */
	UPROPERTY(EditAnywhere, Category = "Button Icon")
		UTexture2D* GamepadButtonIcon;

	UPROPERTY(Transient)
		UMaterialInterface* Material = nullptr;

	UPROPERTY(Transient)
		UMaterialInstanceDynamic* DynamicMaterial = nullptr;
};