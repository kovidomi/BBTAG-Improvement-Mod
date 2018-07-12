#pragma once
#include "CharInfo.h"
#include "CharPaletteHandle.h"

class CharHandle
{
	CharInfo* m_charData;
	CharPaletteHandler m_charPalHandler;

	void SetCharDataPtr(const void* addr);
	const CharInfo* Data() const;
	const CharPaletteHandler& PalHandle() const;
};