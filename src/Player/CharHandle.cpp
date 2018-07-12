#include "CharHandle.h"

void CharHandle::SetCharDataPtr(const void * addr)
{
	m_charData = (CharInfo*)addr;
}

const CharInfo * CharHandle::Data() const
{
	return m_charData;
}

const CharPaletteHandler & CharHandle::PalHandle() const
{
	return m_charPalHandler;
}
