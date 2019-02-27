#pragma once
#include "Window.h"

#include "Core/interfaces.h"
#include "Game/characters.h"
#include "Game/CharHandle.h"
#include "Game/CharPaletteHandle.h"

#include <vector>

class PaletteEditorWindow : public Window
{
public:
	PaletteEditorWindow(const std::string& windowTitle, bool windowClosable,
		ImGuiWindowFlags windowFlags = 0)
		: Window(windowTitle, windowClosable, windowFlags),
		  m_customPaletteVector(g_interfaces.pPaletteManager->GetCustomPalettesVector())
	{
		OnMatchInit();
	}

	~PaletteEditorWindow() override = default;

	void ShowAllPaletteSelections();
	void ShowReloadAllPalettesButton();
	void OnMatchInit();

protected:
	void Draw() override;

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

	std::vector<std::vector<IMPL_data_t>>& m_customPaletteVector;
	CharHandle*         m_allSelectedCharHandles[4];
	const char*         m_allSelectedCharNames[4];
	const char*         m_selectedCharName;
	CharPaletteHandle*  m_selectedCharPalHandle;
	CharIndex           m_selectedCharIndex;
	int                 m_selectedPalIndex;
	PaletteFile         m_selectedFile;
	char                m_paletteEditorArray[1024];
	char                m_highlightArray[IMPL_PALETTE_DATALEN];
	int                 m_colorEditFlags;
	bool                m_highlightMode;
	bool                m_showAlpha;
	bool                m_showIndexes;
};
