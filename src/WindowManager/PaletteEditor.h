#pragma once
#include "../Game/CharPaletteHandle.h"
#include "../Game/CharHandle.h"
#include "../Game/characters.h"
#include <vector>

class PaletteEditor
{
private:
	std::vector<std::vector<IMPL_data_t>> customPaletteVector;

	const char* allSelectedCharNames[4];
	const char* selectedCharName;

	CharPaletteHandle* allSelectedCharPalInfos[4];
	CharPaletteHandle* selectedCharPalInfo;

	const CharInfo* allSelectedCharObjs[4];

	CharIndex selectedCharIndex;
	int selectedPalIndex;
	PaletteFile selectedFile;

	char paletteEditorArray[1024];
	char highlightArray[IMPL_PALETTE_DATALEN];

	int color_edit_flags;
	bool highlight_mode;
	bool show_alpha;

public:
	PaletteEditor();
	void ShowPaletteEditorWindow(bool* p_open);
	void ShowAllPaletteSelections();
	void ShowReloadAllPalettesButton();
	void OnMatchInit();

private:
	bool HasNullPointer();
	void InitializeSelectedCharacters();
	void CharacterSelection();
	void PaletteSelection();
	void FileSelection();
	void EditingModesSelection();
	void ShowPaletteBoxes();
	void DisableHighlightModes();
	void SavePaletteToFile();
	void CheckSelectedPalOutOfBound();
	void ShowPaletteSelect(CharHandle * charHandle, const char* btnText, const char* popupID);
	void ShowHoveredPaletteToolTip(CharIndex charIndex, int palIndex);
	void CopyToEditorArray(char* pSrc);
	void UpdateHighlightArray(int selectedBoxIndex);
};