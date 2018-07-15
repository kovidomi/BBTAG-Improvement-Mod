#pragma once
#include "impl_format.h"
#include "../Game/CharPaletteHandle.h"
#include "../Game/CharHandle.h"
#include "../Game/characters.h"
#include <vector>

class PaletteManager
{
private:
	std::vector<std::vector<IMPL_data_t>> m_customPalettes;
	std::vector<std::vector<std::string>> m_paletteSlots;
public:
	PaletteManager();
	~PaletteManager();
	std::vector<std::vector<IMPL_data_t>> &GetCustomPalettesVector();

	void LoadImplFile(IMPL_t &filledPal);
	bool WritePaletteToFile(CharIndex charIndex, IMPL_data_t *filledPalData);

	void LoadAllPalettes();
	void ReloadAllPalettes();

	int FindCustomPalIndex(CharIndex charIndex, const char* palNameToFind);
	bool SwitchPalette(CharIndex charIndex, CharPaletteHandle& palHandle, int newCustomPalIndex);
	void ReplacePaletteFile(const char* newPalData, PaletteFile palFile, CharPaletteHandle& palHandle);
	char* GetPalFileAddr(PaletteFile palFile, CharPaletteHandle& palHandle);
	int GetCurrentCustomPalIndex(CharPaletteHandle& palHandle);
	const IMPL_data_t &GetCurrentPalData(CharPaletteHandle& palHandle);
	//Remember to call it ONCE per frame 
	void OnUpdate(CharPaletteHandle& P1Ch1, CharPaletteHandle& P1Ch2, CharPaletteHandle& P2Ch1, CharPaletteHandle& P2Ch2);
	//Remember to call it ONCE upon match start
	void OnMatchInit(CharHandle& P1Ch1, CharHandle& P1Ch2, CharHandle& P2Ch1, CharHandle& P2Ch2);

private:
	void CreatePaletteFolders();
	void InitCustomPaletteVector();
	void LoadPalettesIntoVector(CharIndex charIndex, std::wstring& wFolderPath);
	void LoadPalettesFromFolder();
	void ReloadPalettesFromFolder();

	bool CreatePaletteSlotsFile();
	void InitPaletteSlotsVector();
	void LoadPaletteSlotsFile();
	void ReloadPaletteSlotsFile();

	void ApplyPaletteSlot(CharIndex charIndex, CharPaletteHandle& charPalHandle);
};