// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveDustyUserWidget.h"
#include "Components/Image.h"
#include "Dusty/Utils/DustyUtils.h"

void USaveDustyUserWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
    Super::NativeTick(MyGeometry, DeltaTime);

    TimeBetweenFades += DeltaTime;

    // Easing function for smooth transition
    float EasedOpacity = FMath::InterpEaseInOut(0.f, 1.f, TimeBetweenFades / TimerBetweenFades, 2.f);

    // Set the opacity for the main saving image
    FLinearColor Color = SavingImage->GetColorAndOpacity();
    Color.A = FMath::Clamp(EasedOpacity, 0.0f, 1.0f);  // Clamp to ensure it's within the valid range
    SavingImage->SetColorAndOpacity(Color);

    // Set the opacity for the auxiliary image (inverse of the main image's opacity)
    FLinearColor AuxColor = AuxSavingImage->GetColorAndOpacity();
    float AuxOpacity = 1 - EasedOpacity;
    AuxColor.A = FMath::Clamp(AuxOpacity, 0.0f, 1.0f);
    AuxSavingImage->SetColorAndOpacity(AuxColor);

    // Only update the image textures if the fade cycle is complete
    if (TimeBetweenFades >= TimerBetweenFades)
    {
        TimeBetweenFades = 0;

        // Cycle to the next image in the array
        SavingImageIndex = (SavingImageIndex + 1) % SavingImages.Num();
        SavingImage->SetBrushFromTexture(SavingImages[SavingImageIndex]);

        // Set the auxiliary image to the next one in the sequence
        AuxSavingImage->SetBrushFromTexture(SavingImages[(SavingImageIndex + 1) % SavingImages.Num()]);
    }
}
