#pragma once
#include "impl_format.h"
#include "CharPalInfo.h"
#include "../charobj.h"
#include <vector>

class PaletteManager
{
private:
	std::vector<std::vector<IMPL_data_t>> m_customPalettes;

public:
	PaletteManager();
	~PaletteManager();
	std::vector<std::vector<IMPL_data_t>> &GetCustomPalettesVector();
	bool WritePaletteToFile(CharIndex charIndex, IMPL_data_t *filledPalData);
	void ReloadPalettesFromFolder();
	void LoadPalettesFromFolder();
	bool SwitchPalette(CharIndex charIndex, int customPalIndex, CharPalInfo* charPalInfoObj);
	void ReplacePaletteFile(char* newPalData, PaletteFile palFile, CharPalInfo* charPalInfoObj);
	char* GetPalFileAddr(PaletteFile palFile, CharPalInfo* charPalInfoObj);
	int GetCurrentCustomPalIndex(CharPalInfo* charPalInfoObj);
	IMPL_data_t &GetCurrentPalData(CharPalInfo* charPalInfoObj);
	//Remember to call it ONCE per frame 
	void UnlockUpdates(CharPalInfo* P1Ch1, CharPalInfo* P1Ch2, CharPalInfo* P2Ch1, CharPalInfo* P2Ch2);
	//Remember to call it ONCE upon match start
	void OnMatchInit(CharPalInfo* P1Ch1, CharPalInfo* P1Ch2, CharPalInfo* P2Ch1, CharPalInfo* P2Ch2);

private:
	void CreatePaletteFolders();
	void InitCustomPaletteVector();
	void LoadPalettesIntoVector(CharIndex charIndex, const wchar_t* path);
};