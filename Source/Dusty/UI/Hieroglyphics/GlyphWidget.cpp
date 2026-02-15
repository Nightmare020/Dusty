// Fill out your copyright notice in the Description page of Project Settings.


#include "GlyphWidget.h"
#include "Components/Image.h"

void UGlyphWidget::SetGlyph(EGlyphs _glyph)
{
	for (int i = 0; i < m_glyphList.Num(); i++)
	{
		if (_glyph == m_glyphList[i].type)
		{
			m_glyph->SetBrushFromTexture(m_glyphList[i].texture, true);
			return;
		}
	}
}