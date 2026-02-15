// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../../Utils/DustyConst.h"
#include "GlyphWidget.generated.h"

class UImage;

USTRUCT()
struct FGlyphImage
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere)
	EGlyphs type = EGlyphs::House;
	UPROPERTY(EditAnywhere, Transient)
	UTexture2D* texture = nullptr;
};

UCLASS()
class DUSTY_API UGlyphWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget), Transient)
		UImage* m_glyph = nullptr;

	UPROPERTY(EditAnywhere, category = "Glyphs", meta=(DisplayName="Glyph List"))
		TArray<FGlyphImage> m_glyphList;

public:
	void SetGlyph(EGlyphs _glyph);
};
