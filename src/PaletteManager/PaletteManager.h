#pragma once
#include "impl_format.h"
#include "../Game/CharPaletteHandle.h"
#include "../Game/characters.h"
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
	bool SwitchPalette(CharIndex charIndex, int customPalIndex, CharPaletteHandle* palHandle);
	void ReplacePaletteFile(char* newPalData, PaletteFile palFile, CharPaletteHandle* palHandle);
	char* GetPalFileAddr(PaletteFile palFile, CharPaletteHandle* palHandle);
	int GetCurrentCustomPalIndex(CharPaletteHandle* palHandle);
	const IMPL_data_t &GetCurrentPalData(CharPaletteHandle* palHandle);
	//Remember to call it ONCE per frame 
	void UnlockUpdates(CharPaletteHandle& P1Ch1, CharPaletteHandle& P1Ch2, CharPaletteHandle& P2Ch1, CharPaletteHandle& P2Ch2);
	//Remember to call it ONCE upon match start
	void OnMatchInit(CharPaletteHandle& P1Ch1, CharPaletteHandle& P1Ch2, CharPaletteHandle& P2Ch1, CharPaletteHandle& P2Ch2);

private:
	void CreatePaletteFolders();
	void InitCustomPaletteVector();
	void LoadPalettesIntoVector(CharIndex charIndex, const wchar_t* path);
};