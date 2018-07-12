#include "CharHandle.h"

void CharHandle::SetCharDataPtr(const void * addr)
{
	m_charData = (CharInfo**)addr;
}

bool CharHandle::IsNullPtrCharData() const
{
	if (m_charData == 0)
		return true;

	return *m_charData == 0;
}

const CharInfo * CharHandle::Data() const
{
	return *m_charData;
}

CharPaletteHandle & CharHandle::PalHandle()
{
	return m_charPalHandler;
}
