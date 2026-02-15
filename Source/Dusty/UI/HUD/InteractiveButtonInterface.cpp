// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveButtonInterface.h"
//#include "CommonInputTypeEnum.h"
#include "Dusty/Utils/DustyUtils.h"
#include "Dusty/Utils/DustyConst.h"

UInteractiveButtonInterface::UInteractiveButtonInterface()
{
	// Set this element to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryComponentTick.bCanEverTick = true;
}


void UInteractiveButtonInterface::SetMaterialAlpha(float Alpha)
{
	if (DynamicMaterial)
	{
		DynamicMaterial->SetScalarParameterValue(AlphaParamHUD, Alpha);
	}
}

float UInteractiveButtonInterface::GetMaterialAlpha()
{
	if (DynamicMaterial)
	{
		float alpha = 0;
		DynamicMaterial->GetScalarParameterValue(AlphaParamHUD, alpha);
		return alpha;
	}
	return 0;
}

FVector2D UInteractiveButtonInterface::GetMaterialScale()
{
	if (Elements.Num() != 0)
	{
		//https://forums.unrealengine.com/t/how-can-i-set-billboard-elements-0-basesizex-y-values-using-blueprints/282358/11
		ToggleVisibility();
		ToggleVisibility();

		return FVector2D(Elements[0].BaseSizeX, Elements[0].BaseSizeY);
	}
	return FVector2D();
}

void UInteractiveButtonInterface::SetMaterialScale(FVector2D Scale)
{
	if (Elements.Num() != 0)
	{
		Elements[0].BaseSizeX = Scale.X;
		Elements[0].BaseSizeY = Scale.Y;

		//https://forums.unrealengine.com/t/how-can-i-set-billboard-elements-0-basesizex-y-values-using-blueprints/282358/11
		ToggleVisibility();
		ToggleVisibility();
	}
}

void UInteractiveButtonInterface::BeginPlay()
{
	Super::BeginPlay();

	if (Elements.Num() != 0)
	{
		Material = Elements[0].Material;
		DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);
		Elements[0].Material = DynamicMaterial;
		DynamicMaterial->SetScalarParameterValue(AlphaParamHUD, 0);
	}

	// Update rhe icon button on begin
	UpdateButtonIcon();
}

void UInteractiveButtonInterface::UpdateButtonIcon()
{
	ECommonInputType InputType = DustyUtils::GetCurrentInputType();

	// Check if the input is Keyboard/Mouse or Gamepad
	if (InputType == ECommonInputType::MouseAndKeyboard)
	{
		if (DynamicMaterial)
		{
			DynamicMaterial->SetTextureParameterValue(SpriteParamHUD, KeyboardMouseButtonIcon);
		}
		//SetSprite(KeyboardMouseButtonIcon);
	}
	else if (InputType == ECommonInputType::Gamepad)
	{
		if (DynamicMaterial)
		{
			DynamicMaterial->SetTextureParameterValue(SpriteParamHUD, GamepadButtonIcon);
		}
		//SetSprite(GamepadButtonIcon);
	}
}
//
//ECommonInputType UInteractiveButtonInterface::GetCurrentInputType() const
//{
//	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
//
//	if (PlayerController)
//	{
//		const ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
//
//		if (LocalPlayer)
//		{
//			UCommonInputSubsystem* InputSubsystem = UCommonInputSubsystem::Get(LocalPlayer);
//
//			if (InputSubsystem)
//			{
//				return InputSubsystem->GetCurrentInputType();
//			}
//		}
//	}
//
//	// Default to Keyboard if no input is detected
//	return ECommonInputType::MouseAndKeyboard;
//}
