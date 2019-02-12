#pragma once
#include "Game/CharPaletteHandle.h"
#include "Game/CharHandle.h"
#include "Game/characters.h"
#include <vector>

class PaletteEditor
{
private:
	std::vector<std::vector<IMPL_data_t>>& customPaletteVector;

	CharHandle* allSelectedCharHandles[4];
	const char* allSelectedCharNames[4];

	const char* selectedCharName;
	CharPaletteHandle* selectedCharPalHandle;
	CharIndex selectedCharIndex;
	int selectedPalIndex;
	PaletteFile selectedFile;

	char paletteEditorArray[1024];
	char highlightArray[IMPL_PALETTE_DATALEN];

	int color_edit_flags;
	bool highlight_mode;
	bool show_alpha;
	bool show_indexes;

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
	void ReloadSavedPalette(const char* palName);
	bool ShowOverwritePopup(bool *p_open, const wchar_t* wFullPath, const char* filename);
	void CheckSelectedPalOutOfBound();
	void ShowPaletteSelectButton(CharHandle & charHandle, const char* btnText, const char* popupID);
	void ShowPaletteSelectPopup(CharPaletteHandle& charPalHandle, CharIndex charIndex, const char* popupID);
	void ShowHoveredPaletteToolTip(CharPaletteHandle& charPalHandle, CharIndex charIndex, int palIndex);
	void HandleHoveredPaletteSelection(CharPaletteHandle* charPalHandle, CharIndex charIndex, int palIndex, const char* popupID, bool pressed);
	void ShowPaletteRandomizerButton(const char * btnID, CharHandle& charHandle);
	void CopyToEditorArray(const char* pSrc);
	void CopyPalFileToEditorArray(PaletteFile palFile, CharPaletteHandle &charPalHandle);
	void UpdateHighlightArray(int selectedBoxIndex);
	void CopyPalTextsToTextBoxes(CharPaletteHandle& charPalHandle);
	void ShowGradientPopup();
	void GenerateGradient(int idx1, int idx2, int color1, int color2);
};