#include "CharPalInfo.h"
#include "../logger.h"

char* palFileNames[TOTAL_PALETTE_FILES] = { "Character", "Effect01", "Effect02", "Effect03", "Effect04", "Effect05", "Effect06", "Effect07" };

char * CharPalInfo::GetPalFileAddr(char * base, int palIndex, int fileID)
{
	//dereferencing the multi-level pointer:
	// [[[[baseaddr] + 0x4] + palIndex * 0x20] + fileID * 0x4] + 0x1C

	return (char*)(*((int*)*((int*)(*(int*)m_pPalBaseAddr) + 1) + (palIndex * 8) + fileID) + 0x1C);
}

void CharPalInfo::SetPointerPalIndex(int* pPalIndex)
{
	m_pCurPalIndex = pPalIndex;
}

void CharPalInfo::SetPointerBasePal(char* pPalBaseAddr)
{
	m_pPalBaseAddr = pPalBaseAddr;
}

void CharPalInfo::SetPaletteIndex(int palIndex)
{
	if (palIndex < 0 || palIndex > MAX_PAL_INDEX)
		return;

	*m_pCurPalIndex = palIndex;
}

bool CharPalInfo::IsNullPointerPalIndex()
{
	if(m_pCurPalIndex)
		return false;

	return true;
}

int & CharPalInfo::GetPalIndexRef()
{
	return *m_pCurPalIndex;
}

void CharPalInfo::ReplaceAllPalFiles(IMPL_data_t *newPaletteData)
{
	ReplaceAllPalFiles(newPaletteData, m_switchPalIndex1);
	ReplaceAllPalFiles(newPaletteData, m_switchPalIndex2);

	UpdatePalette();
}

void CharPalInfo::ReplaceSinglePalFile(char * newPalData, PaletteFile palFile)
{
	char* pDst1 = GetPalFileAddr(m_pPalBaseAddr, m_switchPalIndex1, (int)palFile);
	char* pDst2 = GetPalFileAddr(m_pPalBaseAddr, m_switchPalIndex2, (int)palFile);
	ReplacePalArrayInMemory(pDst1, newPalData);
	ReplacePalArrayInMemory(pDst2, newPalData);

	UpdatePalette();
}

void CharPalInfo::OnMatchInit()
{
	BackupCurrentPal();

	m_selectedCustomPalIndex = 0;

	m_switchPalIndex1 = *m_pCurPalIndex;
	if (m_switchPalIndex1 == MAX_PAL_INDEX)
		m_switchPalIndex2 = m_switchPalIndex1 - 1; 
	else
		m_switchPalIndex2 = m_switchPalIndex1 + 1;
}

void CharPalInfo::UnlockUpdate()
{
	m_updateLocked = false;
}

int CharPalInfo::GetSelectedCustomPalIndex()
{
	return m_selectedCustomPalIndex;
}

void CharPalInfo::SetSelectedCustomPalIndex(int index)
{
	m_selectedCustomPalIndex = index;
}

char * CharPalInfo::GetOrigPalFileAddr(PaletteFile palFile)
{
	return GetPalFileAddr(m_pPalBaseAddr, m_switchPalIndex1, (int)palFile);
}

IMPL_data_t & CharPalInfo::GetCurrentPalData()
{
	for (int i = 0; i < TOTAL_PALETTE_FILES; i++)
	{
		char* pDst = m_CurrentPalData.file0 + (i * IMPL_PALETTE_DATALEN);
		char* pSrc = GetPalFileAddr(m_pPalBaseAddr, m_switchPalIndex1, i);
		memcpy(pDst, pSrc, IMPL_PALETTE_DATALEN);
		pDst += IMPL_PALETTE_DATALEN;
	}

	return m_CurrentPalData;
}

void CharPalInfo::ReplacePalArrayInMemory(char * Dst, const void * Src)
{
	//The palette datas are duplicated in the memory
	//The duplication is found at offset 0x800
	//We have to overwrite both, as some characters use the duplication's address instead
	memcpy(Dst, Src, IMPL_PALETTE_DATALEN);
	memcpy(Dst + 0x800, Src, IMPL_PALETTE_DATALEN);
}

void CharPalInfo::ReplaceAllPalFiles(IMPL_data_t *newPaletteData, int palIndex)
{
	static const char NULLBLOCK[IMPL_PALETTE_DATALEN]{ 0 };

	//If palname is "Default" then we load the original palette from backup
	if (strncmp(newPaletteData->palname, "Default", 32) == 0)
		newPaletteData = &m_origPalBackup;

	for (int i = 0; i < TOTAL_PALETTE_FILES; i++)
	{
		char* pSrc = newPaletteData->file0 + (i * IMPL_PALETTE_DATALEN);
		if (!memcmp(NULLBLOCK, pSrc, IMPL_PALETTE_DATALEN))
			continue;
		char* pDst = GetPalFileAddr(m_pPalBaseAddr, palIndex, i);
		ReplacePalArrayInMemory(pDst, pSrc);
	}
}

void CharPalInfo::BackupCurrentPal()
{
	LOG(2, "BackupCurrentPalette\n");

	char* pSrc = 0;
	char* pDst = m_origPalBackup.file0;

	for(int i = 0; i < TOTAL_PALETTE_FILES; i++)
	{
		pSrc = GetPalFileAddr(m_pPalBaseAddr, *m_pCurPalIndex, i);
		memcpy(pDst, pSrc, IMPL_PALETTE_DATALEN);
		pDst += IMPL_PALETTE_DATALEN;
	}
}

void CharPalInfo::UpdatePalette()
{
	//Must not switch more than once per frame, or palette doesn't get updated!
	if (m_updateLocked)
		return;

	if (*m_pCurPalIndex == m_switchPalIndex1)
		*m_pCurPalIndex = m_switchPalIndex2;
	else
		*m_pCurPalIndex = m_switchPalIndex1;

	LockUpdate();
}

void CharPalInfo::LockUpdate()
{
	m_updateLocked = true;
}
