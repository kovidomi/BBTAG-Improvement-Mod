#pragma once
#include "impl_format.h"
#include "../utils.h"

#define MAX_PAL_INDEX 15
#define TOTAL_PALETTE_FILES 8

extern char* palFileNames[TOTAL_PALETTE_FILES];

enum PaletteFile
{
	PaletteFile_Character,
	PaletteFile_Effect1,
	PaletteFile_Effect2,
	PaletteFile_Effect3,
	PaletteFile_Effect4,
	PaletteFile_Effect5,
	PaletteFile_Effect6,
	PaletteFile_Effect7
};

class CharPalInfo
{
	friend class PaletteManager;

private:
	int* m_pCurPalIndex;
	char* m_pPalBaseAddr;
	IMPL_data_t m_origPalBackup;
	IMPL_data_t m_CurrentPalData;
	int m_switchPalIndex1;
	int m_switchPalIndex2;
	int m_selectedCustomPalIndex;
	bool m_updateLocked;
	
public:
	void SetPointerPalIndex(int* pPalIdx);
	void SetPointerBasePal(char* pPalBaseAddr);
	bool IsNullPointerPalIndex();

private:
	void SetPaletteIndex(int palIndex);
	int& GetPalIndexRef();
	void ReplaceAllPalFiles(IMPL_data_t *newPaletteData);
	void ReplaceSinglePalFile(char* newPalData, PaletteFile palFile);
	void OnMatchInit();
	void UnlockUpdate();
	int GetSelectedCustomPalIndex();
	void SetSelectedCustomPalIndex(int index);
	char* GetOrigPalFileAddr(PaletteFile palFile);
	IMPL_data_t& GetCurrentPalData();
	char* GetPalFileAddr(char* base, int palIdx, int fileIdx);
	void ReplacePalArrayInMemory(char* Dst, const void* Src);
	void ReplaceAllPalFiles(IMPL_data_t *newPaletteData, int palIdx);
	void BackupCurrentPal();
	void UpdatePalette();
	void LockUpdate();
};

struct CharPalInfos
{
	CharPalInfo* P1Char1;
	CharPalInfo* P1Char2;
	CharPalInfo* P2Char1;
	CharPalInfo* P2Char2;
	CharPalInfos()
	{
		P1Char1 = new CharPalInfo();
		P1Char2 = new CharPalInfo();
		P2Char1 = new CharPalInfo();
		P2Char2 = new CharPalInfo();
	}
	~CharPalInfos()
	{
		SAFE_DELETE(P1Char1);
		SAFE_DELETE(P1Char2);
		SAFE_DELETE(P2Char1);
		SAFE_DELETE(P2Char2);
	}
};