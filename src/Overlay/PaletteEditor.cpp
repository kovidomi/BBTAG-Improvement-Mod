#include "PaletteEditor.h"

#include "WindowManager.h"

#include "Core/interfaces.h"
#include "Core/logger.h"

#include "Game/gamestates.h"
#include "PaletteManager/impl_format.h"

#include <Shlwapi.h>
#include <imgui.h>

#define NUMBER_OF_COLOR_BOXES (IMPL_PALETTE_DATALEN / sizeof(int)) //256
#define COLUMNS 16
const int COLOR_BLACK = 0xFF000000;
const int COLOR_WHITE = 0xFFFFFFFF;
const ImVec4 COLOR_DONATOR(1.000f, 0.794f, 0.000f, 1.000f);
const ImVec4 COLOR_ONLINE(0.260f, 0.590f, 0.980f, 1.000f);

static char palNameBuf[IMPL_PALNAME_LENGTH] = "";
static char palDescBuf[IMPL_DESC_LENGTH] = "";
static char palCreatorBuf[IMPL_CREATOR_LENGTH] = "";

PaletteEditor::PaletteEditor() : customPaletteVector(g_interfaces.pPaletteManager->GetCustomPalettesVector())
{
	OnMatchInit();
}

void PaletteEditor::ShowPaletteEditorWindow(bool* p_open)
{
	if (*g_gameVals.pGameMode != GameMode_Training)
		return;

	if (HasNullPointer())
		return;

	ImGui::Begin("Palette editor", p_open);

	CheckSelectedPalOutOfBound();

	CharacterSelection();
	PaletteSelection();
	FileSelection();
	EditingModesSelection();
	ShowPaletteBoxes();
	SavePaletteToFile();

	ImGui::End();
}

void PaletteEditor::ShowAllPaletteSelections()
{
	if (HasNullPointer())
		return;

	ShowPaletteSelectButton(g_interfaces.player1.GetChar1(), "P1Ch1 palette", "select1-1");
	ShowPaletteSelectButton(g_interfaces.player1.GetChar2(), "P1Ch2 palette", "select1-2");
	ShowPaletteSelectButton(g_interfaces.player2.GetChar1(), "P2Ch1 palette", "select2-1");
	ShowPaletteSelectButton(g_interfaces.player2.GetChar2(), "P2Ch2 palette", "select2-2");
}

void PaletteEditor::ShowReloadAllPalettesButton()
{
	if (ImGui::Button("Reload custom palettes"))
	{
		g_interfaces.pPaletteManager->ReloadAllPalettes();
	}
}

void PaletteEditor::OnMatchInit()
{
	if (HasNullPointer())
		return;

	InitializeSelectedCharacters();

	customPaletteVector = g_interfaces.pPaletteManager->GetCustomPalettesVector();

	selectedCharIndex = (CharIndex)allSelectedCharHandles[0]->GetData()->char_index;
	selectedCharName = allSelectedCharNames[0];
	selectedCharPalHandle = &allSelectedCharHandles[0]->GetPalHandle();
	selectedPalIndex = g_interfaces.pPaletteManager->GetCurrentCustomPalIndex(*selectedCharPalHandle);
	CopyPalTextsToTextBoxes(*selectedCharPalHandle);
	selectedFile = PaletteFile_Character;

	color_edit_flags = ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoAlpha;
	highlight_mode = false;
	show_alpha = false;

	CopyPalFileToEditorArray(selectedFile, *selectedCharPalHandle);

	g_gameVals.isPaletteModePaused = false;
}

bool PaletteEditor::HasNullPointer()
{
	if (g_interfaces.player1.GetChar1().IsCharDataNullPtr() ||
		g_interfaces.player1.GetChar2().IsCharDataNullPtr() ||
		g_interfaces.player2.GetChar1().IsCharDataNullPtr() ||
		g_interfaces.player2.GetChar2().IsCharDataNullPtr())
	{
		return true;
	}

	return false;
}

void PaletteEditor::InitializeSelectedCharacters()
{
	allSelectedCharHandles[0] = &g_interfaces.player1.GetChar1();
	allSelectedCharHandles[1] = &g_interfaces.player1.GetChar2();
	allSelectedCharHandles[2] = &g_interfaces.player2.GetChar1();
	allSelectedCharHandles[3] = &g_interfaces.player2.GetChar2();

	allSelectedCharNames[0] = charNames[allSelectedCharHandles[0]->GetData()->char_index].c_str();
	allSelectedCharNames[1] = charNames[allSelectedCharHandles[1]->GetData()->char_index].c_str();
	allSelectedCharNames[2] = charNames[allSelectedCharHandles[2]->GetData()->char_index].c_str();
	allSelectedCharNames[3] = charNames[allSelectedCharHandles[3]->GetData()->char_index].c_str();
}

void PaletteEditor::CharacterSelection()
{
	LOG(7, "PaletteEditor CharacterSelection\n");

	if (ImGui::Button("Select character"))
		ImGui::OpenPopup("select_char_pal");
	ImGui::SameLine();

	ImGui::Text(selectedCharName);

	if (ImGui::BeginPopup("select_char_pal"))
	{
		const int NUMBER_OF_CHARS = 4;
		
		for (int i = 0; i < NUMBER_OF_CHARS; i++)
		{
			ImGui::PushID(i);
			if (ImGui::Selectable(allSelectedCharNames[i]))
			{
				DisableHighlightModes();

				selectedCharIndex = (CharIndex)allSelectedCharHandles[i]->GetData()->char_index;
				selectedCharName = allSelectedCharNames[i];
				selectedCharPalHandle = &allSelectedCharHandles[i]->GetPalHandle();
				selectedPalIndex = g_interfaces.pPaletteManager->GetCurrentCustomPalIndex(*selectedCharPalHandle);
				CopyPalFileToEditorArray(selectedFile, *selectedCharPalHandle);
			}
			ImGui::PopID();
		}
		ImGui::EndPopup();
	}
}

void PaletteEditor::PaletteSelection()
{
	LOG(7, "PaletteEditor PaletteSelection\n");

	if (ImGui::Button("Select palette  "))
		ImGui::OpenPopup("select_custom_pal");

	ImGui::SameLine();
	ImGui::Text(customPaletteVector[selectedCharIndex][selectedPalIndex].palname);

	ShowPaletteSelectPopup(*selectedCharPalHandle, selectedCharIndex, "select_custom_pal");
}

void PaletteEditor::FileSelection()
{
	LOG(7, "PaletteEditor FileSelection\n");

	if (ImGui::Button("Select file     "))
		ImGui::OpenPopup("select_file_pal");
	ImGui::SameLine();

	ImGui::Text(palFileNames[selectedFile]);

	if (ImGui::BeginPopup("select_file_pal"))
	{
		for (int i = 0; i < TOTAL_PALETTE_FILES; i++)
		{
			if (ImGui::Selectable(palFileNames[i]))
			{
				DisableHighlightModes();
				selectedFile = (PaletteFile)(i);
				CopyPalFileToEditorArray(selectedFile, *selectedCharPalHandle);
			}
		}
		ImGui::EndPopup();
	}
}

void PaletteEditor::EditingModesSelection()
{
	LOG(7, "PaletteEditor EditingModesSelection\n");

	ImGui::Separator();
	if (ImGui::Checkbox("Show transparency values", &show_alpha))
	{
		if (show_alpha)
			color_edit_flags = ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_AlphaBar;
		else
			color_edit_flags = ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoAlpha;
	}

	ImGui::SameLine();
	int nextLineColumnPosX = ImGui::GetCursorPosX();
	ImGui::Checkbox("Freeze frame", &g_gameVals.isPaletteModePaused);
	
	if (ImGui::Checkbox("Highlight mode", &highlight_mode))
	{
		if (highlight_mode)
		{
			//fill the array with black
			for (int i = 0; i < NUMBER_OF_COLOR_BOXES; i++)
			{
				((int*)highlightArray)[i] = COLOR_BLACK;
			}
			g_interfaces.pPaletteManager->ReplacePaletteFile(highlightArray, selectedFile, *selectedCharPalHandle);
		}
		else
		{
			DisableHighlightModes();
		}
	}

	ImGui::SameLine();
	ImGui::SetCursorPosX(nextLineColumnPosX);
	ImGui::Checkbox("Show indexes", &show_indexes);

	if (ImGui::Button("Gradient generator"))
		ImGui::OpenPopup("gradient");
	ShowGradientPopup();

	ImGui::Separator();
}

void PaletteEditor::ShowPaletteBoxes()
{
	LOG(7, "PaletteEditor ShowPaletteBoxes\n");

	ImGui::Text("");
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2, 2));

	if (show_indexes)
	{
		ImGui::TextUnformatted("001 "); ImGui::SameLine();
	}

	for (int i = 0, col = 1; i < NUMBER_OF_COLOR_BOXES; i++)
	{
		ImGui::PushID(i);

		bool pressed = false;
		int curColorBoxOffset = (i * sizeof(int));

		if (highlight_mode)
			pressed = ImGui::ColorButtonOn32Bit("##PalColorButton", (unsigned char*)paletteEditorArray + curColorBoxOffset, color_edit_flags);
		else
			pressed = ImGui::ColorEdit4On32Bit("##PalColorEdit", (unsigned char*)paletteEditorArray + curColorBoxOffset, color_edit_flags);

		if (pressed)
		{
			if (highlight_mode)
			{
				UpdateHighlightArray(i);
			}
			else
			{
				g_interfaces.pPaletteManager->ReplacePaletteFile(paletteEditorArray, selectedFile, *selectedCharPalHandle);
			}
		}

		if (col < COLUMNS)
		{
			//continue the row
			ImGui::SameLine();
			col++;
		}
		else
		{
			//start a new row
			col = 1;
			if (show_indexes && i < NUMBER_OF_COLOR_BOXES - 1)
			{
				ImGui::Text("%.3d ", i + 2);
				ImGui::SameLine();
			}
		}
		ImGui::PopID();
	}
	ImGui::PopStyleVar();
}

void PaletteEditor::DisableHighlightModes()
{
	highlight_mode = false;
	g_interfaces.pPaletteManager->ReplacePaletteFile(paletteEditorArray, selectedFile, *selectedCharPalHandle);
}

void PaletteEditor::SavePaletteToFile()
{
	static char message[200] = "";

	ImGui::Text("");
	ImGui::Separator();

	if (highlight_mode)
	{
		ImGui::TextDisabled("Cannot save with Highlight mode on!");
		return;
	}

	struct TextFilters { static int FilterAllowedChars(ImGuiTextEditCallbackData* data) { if (data->EventChar < 256 && strchr(" qwertzuiopasdfghjklyxcvbnmQWERTZUIOPASDFGHJKLYXCVBNM0123456789_.()[]!@&+-'^,;{}$=", (char)data->EventChar)) return 0; return 1; } };

	ImGui::Text("Palette name:");
	ImGui::PushItemWidth(250);
	ImGui::InputText("##palName", palNameBuf, IMPL_PALNAME_LENGTH, ImGuiInputTextFlags_CallbackCharFilter, TextFilters::FilterAllowedChars);
	ImGui::PopItemWidth();

	ImGui::Text("Creator (optional):");
	ImGui::PushItemWidth(250);
	ImGui::InputText("##palcreator", palCreatorBuf, IMPL_CREATOR_LENGTH, ImGuiInputTextFlags_CallbackCharFilter, TextFilters::FilterAllowedChars);
	ImGui::PopItemWidth();

	ImGui::Text("Palette description (optional):");
	ImGui::PushItemWidth(250);
	ImGui::InputText("##palDesc", palDescBuf, IMPL_DESC_LENGTH, ImGuiInputTextFlags_CallbackCharFilter, TextFilters::FilterAllowedChars);
	ImGui::PopItemWidth();

	bool pressed = ImGui::Button("Save palette");
	ImGui::Text(message);

	static bool show_overwrite_popup = false;

	if (pressed || show_overwrite_popup)
	{
		if (strncmp(palNameBuf, "", IMPL_PALNAME_LENGTH) == 0)
		{
			memcpy(message, "Error, no filename given", 25);
			WindowManager::AddLog("[error] Could not save custom palette, no filename was given\n");
			return;
		}
		else if (strncmp(palNameBuf, "Default", IMPL_PALNAME_LENGTH) == 0 || strncmp(palNameBuf, "Random", IMPL_PALNAME_LENGTH) == 0)
		{
			memcpy(message, "Error, not a valid filename", 28);
			WindowManager::AddLog("[error] Could not save custom palette: not a valid filename\n");
			return;
		}

		TCHAR pathBuf[MAX_PATH];
		GetModuleFileName(NULL, pathBuf, MAX_PATH);
		std::wstring::size_type pos = std::wstring(pathBuf).find_last_of(L"\\");
		std::wstring wFullPath = std::wstring(pathBuf).substr(0, pos);

		wFullPath += L"\\BBTAG_IM\\Palettes\\";
		wFullPath += wCharNames[selectedCharIndex];
		wFullPath += L"\\";

		std::string filenameTemp(palNameBuf);
		std::wstring wFilename(filenameTemp.begin(), filenameTemp.end());
		wFullPath += wFilename;

		if (wFilename.find(L".impl") == std::wstring::npos)
		{
			wFullPath += L".impl";
			filenameTemp += ".impl";
		}

		if (ShowOverwritePopup(&show_overwrite_popup, wFullPath.c_str(), filenameTemp.c_str()))
		{

			IMPL_data_t curPalData = g_interfaces.pPaletteManager->GetCurrentPalData(*selectedCharPalHandle);

			strncpy(curPalData.creator, palCreatorBuf, IMPL_CREATOR_LENGTH);
			strncpy(curPalData.palname, palNameBuf, IMPL_PALNAME_LENGTH);
			strncpy(curPalData.desc, palDescBuf, IMPL_DESC_LENGTH);

			std::string messageText = "'";
			messageText += filenameTemp.c_str();

			if (g_interfaces.pPaletteManager->WritePaletteToFile(selectedCharIndex, &curPalData))
			{
				std::string fullPath(wFullPath.begin(), wFullPath.end());
				WindowManager::AddLog("[system] Custom palette '%s' successfully saved to:\n'%s'\n", filenameTemp.c_str(), fullPath.c_str());
				messageText += "' saved successfully";

				ReloadSavedPalette(palNameBuf);
			}
			else
			{
				WindowManager::AddLog("[error] Custom palette '%s' failed to be saved.\n", filenameTemp.c_str());
				messageText += "' save failed";
			}

			memcpy(message, messageText.c_str(), messageText.length() + 1);
		}
	}
}

void PaletteEditor::ReloadSavedPalette(const char* palName)
{
	WindowManager::DisableLogging();
	g_interfaces.pPaletteManager->ReloadAllPalettes();
	WindowManager::EnableLogging();

	//find the newly loaded custom pal
	selectedPalIndex = g_interfaces.pPaletteManager->FindCustomPalIndex(selectedCharIndex, palName);

	if (selectedPalIndex < 0)
	{
		WindowManager::AddLog("[error] Saved custom palette couldn't be reloaded. Not found.\n");
		selectedPalIndex = 0;
	}

	g_interfaces.pPaletteManager->SwitchPalette(selectedCharIndex, *selectedCharPalHandle, selectedPalIndex);
	CopyPalFileToEditorArray(selectedFile, *selectedCharPalHandle);
}

bool PaletteEditor::ShowOverwritePopup(bool *p_open, const wchar_t* wFullPath, const char* filename)
{
	bool isOverwriteAllowed = true;

	if (PathFileExists(wFullPath))
	{
		ImGui::OpenPopup("Overwrite?");
		*p_open = true;
	}
	if (ImGui::BeginPopupModal("Overwrite?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("'%s' already exists.\nAre you sure you want to overwrite it?\n\n", filename);
		ImGui::Separator();
		if (ImGui::Button("OK", ImVec2(120, 0)))
		{
			ImGui::CloseCurrentPopup();
			*p_open = false;
			isOverwriteAllowed = true;
			return isOverwriteAllowed;
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			ImGui::CloseCurrentPopup();
			*p_open = false;
			isOverwriteAllowed = false;
		}
		ImGui::EndPopup();

		isOverwriteAllowed = false;
	}

	return isOverwriteAllowed;
}

void PaletteEditor::CheckSelectedPalOutOfBound()
{
	if (selectedPalIndex != 0 && selectedPalIndex >= customPaletteVector[selectedCharIndex].size())
	{
		//reset back to default
		selectedPalIndex = 0;
		g_interfaces.pPaletteManager->SwitchPalette(selectedCharIndex, *selectedCharPalHandle, selectedPalIndex);
		CopyPalFileToEditorArray(selectedFile, *selectedCharPalHandle);
	}
}

void PaletteEditor::ShowPaletteSelectButton(CharHandle & charHandle, const char * btnText, const char * popupID)
{
	CharPaletteHandle& charPalHandle = charHandle.GetPalHandle();
	int selected_pal_index = g_interfaces.pPaletteManager->GetCurrentCustomPalIndex(charPalHandle);
	CharIndex charIndex = (CharIndex)charHandle.GetData()->char_index;

	if (charIndex >= CHAR_NAMES_COUNT - 1)
		return;

	ShowPaletteRandomizerButton(popupID, charHandle);
	ImGui::SameLine();

	if (ImGui::Button(btnText))
		ImGui::OpenPopup(popupID);

	ImGui::SameLine();
	ImGui::TextUnformatted(customPaletteVector[charIndex][selected_pal_index].palname);

	ShowPaletteSelectPopup(charPalHandle, charIndex, popupID);
}

void PaletteEditor::ShowPaletteSelectPopup(CharPaletteHandle& charPalHandle, CharIndex charIndex, const char* popupID)
{
	static int hoveredPalIndex = 0;
	bool pressed = false;
	int onlinePalsStartIndex = g_interfaces.pPaletteManager->GetOnlinePalsStartIndex(charIndex);
	ImGui::SetNextWindowSizeConstraints(ImVec2(-1.0f, 25.0f), ImVec2(-1.0f, 300.0f));

	if (ImGui::BeginPopup(popupID))
	{
		ImGui::TextUnformatted(charNames[charIndex].c_str());
		ImGui::Separator();
		for (int i = 0; i < customPaletteVector[charIndex].size(); i++)
		{
			if (i == onlinePalsStartIndex)
			{
				ImGui::PushStyleColor(ImGuiCol_Separator, COLOR_ONLINE);
				ImGui::Separator();
				ImGui::PopStyleColor();
			}

			if (ImGui::Selectable(customPaletteVector[charIndex][i].palname))
			{
				pressed = true;
				g_interfaces.pPaletteManager->SwitchPalette(charIndex, charPalHandle, i);

				//updating palette editor's array if this is the currently selected character
				if (&charPalHandle == selectedCharPalHandle)
				{
					selectedPalIndex = i;
					CopyPalFileToEditorArray(selectedFile, charPalHandle);
					DisableHighlightModes();

					CopyPalTextsToTextBoxes(charPalHandle);
				}
			}
			if (ImGui::IsItemHovered())
				hoveredPalIndex = i;
			ShowHoveredPaletteToolTip(charPalHandle, charIndex, i);
		}
		ImGui::EndPopup();
	}
	HandleHoveredPaletteSelection(&charPalHandle, charIndex, hoveredPalIndex, popupID, pressed);
}

void PaletteEditor::ShowHoveredPaletteToolTip(CharPaletteHandle& charPalHandle, CharIndex charIndex, int palIndex)
{
	if (!ImGui::IsItemHovered())
		return;

	const char* creatorText = customPaletteVector[charIndex][palIndex].creator;
	const char* descText = customPaletteVector[charIndex][palIndex].desc;
	const int creatorLen = strnlen(creatorText, IMPL_CREATOR_LENGTH);
	const int descLen = strnlen(descText, IMPL_DESC_LENGTH);
	bool isOnlinePal = palIndex >= g_interfaces.pPaletteManager->GetOnlinePalsStartIndex(charIndex);

	if (creatorLen || descLen || isOnlinePal)
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(300.0f);

		if (isOnlinePal)
			ImGui::TextColored(COLOR_ONLINE, "ONLINE PALETTE");
		if (creatorLen)
			ImGui::Text("Creator: %s", creatorText);
		if (descLen)
			ImGui::Text("Description: %s", descText);

		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

void PaletteEditor::HandleHoveredPaletteSelection(CharPaletteHandle* charPalHandle, CharIndex charIndex, int palIndex, const char* popupID, bool pressed)
{
	static CharPaletteHandle* prevCharHndl = 0;
	static int prevPalIndex = 0;
	static int origPalIndex = 0;
	static bool paletteSwitched = false;
	static char popupIDbkp[32];
	const char* palFileAddr = 0;

	if (pressed)
	{
		paletteSwitched = false;
	}
	else if (!ImGui::IsPopupOpen(popupID) && strcmp(popupIDbkp, popupID) == 0 
		&& paletteSwitched && prevCharHndl == charPalHandle && !pressed)
	{
		palFileAddr = g_interfaces.pPaletteManager->GetCustomPalFile(charIndex, origPalIndex, PaletteFile_Character, *charPalHandle);
		g_interfaces.pPaletteManager->ReplacePaletteFile(palFileAddr, PaletteFile_Character, *charPalHandle);
		paletteSwitched = false;
	}
	else if (ImGui::IsPopupOpen(popupID) && prevPalIndex != palIndex)
	{
		if (!paletteSwitched)
			origPalIndex = g_interfaces.pPaletteManager->GetCurrentCustomPalIndex(*charPalHandle);

		palFileAddr = g_interfaces.pPaletteManager->GetCustomPalFile(charIndex, palIndex, PaletteFile_Character, *charPalHandle);
		g_interfaces.pPaletteManager->ReplacePaletteFile(palFileAddr, PaletteFile_Character, *charPalHandle);
		prevPalIndex = palIndex;
		prevCharHndl = charPalHandle;
		paletteSwitched = true;
		strcpy(popupIDbkp, popupID);
	}
}

void PaletteEditor::ShowPaletteRandomizerButton(const char * btnID, CharHandle& charHandle)
{
	int charIndex = charHandle.GetData()->char_index;
	char buf[12];
	sprintf(buf, "?##%s", btnID);
	
	ImGui::Text(" "); ImGui::SameLine();
	if (ImGui::Button(buf) && customPaletteVector[charIndex].size() > 1)
	{
		CharPaletteHandle& charPalHandle = charHandle.GetPalHandle();
		int curPalIndex = g_interfaces.pPaletteManager->GetCurrentCustomPalIndex(charPalHandle);
		int newPalIndex = curPalIndex;

		while (curPalIndex == newPalIndex)
		{
			newPalIndex = rand() % customPaletteVector[charIndex].size();
		}
		g_interfaces.pPaletteManager->SwitchPalette((CharIndex)charIndex, charPalHandle, newPalIndex);
	}
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::Text("Random selection");
		ImGui::EndTooltip();
	}
}

void PaletteEditor::CopyToEditorArray(const char * pSrc)
{
	memcpy(paletteEditorArray, pSrc, IMPL_PALETTE_DATALEN);
}

void PaletteEditor::CopyPalFileToEditorArray(PaletteFile palFile, CharPaletteHandle & charPalHandle)
{
	const char* fileAddr = g_interfaces.pPaletteManager->GetPalFileAddr(palFile, charPalHandle);
	CopyToEditorArray(fileAddr);
}

void PaletteEditor::UpdateHighlightArray(int selectedBoxIndex)
{
	static int selected_highlight_box = 0;

	//setting previously pressed box back to black
	((int*)highlightArray)[selected_highlight_box] = COLOR_BLACK;

	selected_highlight_box = selectedBoxIndex;

	//setting currently pressed box to white
	((int*)highlightArray)[selected_highlight_box] = COLOR_WHITE;

	g_interfaces.pPaletteManager->ReplacePaletteFile(highlightArray, selectedFile, *selectedCharPalHandle);
}

void PaletteEditor::CopyPalTextsToTextBoxes(CharPaletteHandle & charPalHandle)
{
	IMPL_data_t palData = g_interfaces.pPaletteManager->GetCurrentPalData(charPalHandle);
	strncpy(palNameBuf, palData.palname, IMPL_PALNAME_LENGTH);
	strncpy(palDescBuf, palData.desc, IMPL_DESC_LENGTH);
	strncpy(palCreatorBuf, palData.creator, IMPL_CREATOR_LENGTH);
}

void PaletteEditor::ShowGradientPopup()
{
	if (ImGui::BeginPopup("gradient"))
	{
		ImGui::TextUnformatted("Gradient generator");

		static int idx1 = 1;
		static int idx2 = 2;
		int minVal_idx2 = idx1 + 1;

		if (idx2 <= idx1)
			idx2 = minVal_idx2;

		ImGui::SliderInt("Start index", &idx1, 1, NUMBER_OF_COLOR_BOXES - 1);
		ImGui::SliderInt("End index", &idx2, minVal_idx2, NUMBER_OF_COLOR_BOXES);

		static int color1 = 0xFFFFFFFF;
		static int color2 = 0xFFFFFFFF;
		int alpha_flag = color_edit_flags & ImGuiColorEditFlags_NoAlpha;

		ImGui::ColorEdit4On32Bit("Start color", (unsigned char*)&color1, alpha_flag);
		ImGui::ColorEdit4On32Bit("End color", (unsigned char*)&color2, alpha_flag);

		if (ImGui::Button("Swap colors"))
		{
			int temp = color2;
			color2 = color1;
			color1 = temp;
		}

		if (ImGui::Button("Generate gradient"))
		{
			DisableHighlightModes();
			GenerateGradient(idx1, idx2, color1, color2);
		}

		ImGui::EndPopup();
	}
}

void PaletteEditor::GenerateGradient(int idx1, int idx2, int color1, int color2)
{
	idx1 -= 1;
	idx2 -= 1;

	int steps = idx2 - idx1;
	if (steps < 1)
		return;

	float frac = 1.0 / (float)(idx2 - idx1);

	//WindowManager::AddLog("color1: 0x%p, color2: 0x%p\n", color1, color2);

	unsigned char a1 = (color1 & 0xFF000000) >> 24;
	unsigned char a2 = (color2 & 0xFF000000) >> 24;
	unsigned char r1 = (color1 & 0xFF0000) >> 16;
	unsigned char r2 = (color2 & 0xFF0000) >> 16;
	unsigned char g1 = (color1 & 0xFF00) >> 8;
	unsigned char g2 = (color2 & 0xFF00) >> 8;
	unsigned char b1 = color1 & 0xFF;
	unsigned char b2 = color2 & 0xFF;

	//WindowManager::AddLog("a1: 0x%p, a2: 0x%p, r1: 0x%p, r2: 0x%p, g1: 0x%p, g2: 0x%p, b1: 0x%p, b2: 0x%p\n", 
	//	a1, a2, r1, r2, g1, g2, b1, b2);
	//WindowManager::AddLogSeparator();

	((int*)paletteEditorArray)[idx1] = color1;

	for (int i = 1; i <= steps; i++)
	{
		int a = ((int)((a2 - a1) * i * frac + a1) & 0xFF) << 24;
		int r = ((int)((r2 - r1) * i * frac + r1) & 0xFF) << 16;
		int g = ((int)((g2 - g1) * i * frac + g1) & 0xFF) << 8;
		int b = (int)((b2 - b1) * i * frac + b1) & 0xFF;
		int color = r | g | b;

		//WindowManager::AddLog("(color = r | g | b) 0x%p = 0x%p | 0x%p | 0x%p\n", color, r, g, b);
		//WindowManager::AddLog("BEFORE: 0x%p, AFTER: 0x%p\n",
		//	((int*)paletteEditorArray)[idx1 + i], 
		//	color ^ (((int*)paletteEditorArray)[idx1 + i] & a));

		((int*)paletteEditorArray)[idx1 + i] = color ^ ((int*)paletteEditorArray)[idx1 + i] & a;
	}

	g_interfaces.pPaletteManager->ReplacePaletteFile(paletteEditorArray, selectedFile, *selectedCharPalHandle);
}