#pragma once
#include "CharInfo.h"
#include "CharPaletteHandle.h"

class CharHandle
{
	CharInfo** m_charData;
	CharPaletteHandle m_charPalHandler;
	//const char* charName;
	//CharIndex charIndex;

public:
	const CharInfo* GetData() const;
	CharPaletteHandle& GetPalHandle();

	void SetCharDataPtr(const void* addr);
	bool IsNullPtrCharData() const;
};