#include "PaletteEditor.h"
#include "WindowManager.h"
#include "../globals.h"
#include "../logger.h"
#include "../Game/gamestates.h"
#include "../PaletteManager/impl_format.h"
#include <Shlwapi.h>
#include <imgui.h>

#define NUMBER_OF_COLOR_BOXES (IMPL_PALETTE_DATALEN / sizeof(int)) //256
#define COLUMN_LENGTH 16
const int COLOR_BLACK = 0xFF000000;
const int COLOR_WHITE = 0xFFFFFFFF;

PaletteEditor::PaletteEditor()
{
	OnMatchInit();
}

void PaletteEditor::ShowPaletteEditorWindow(bool* p_open)
{
	if (*g_gameVals.pGameMode != GameMode_Tutorial)
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

	ShowPaletteSelect(&g_interfaces.Player1.Char1(), "P1Ch1 palette", "select1-1");
	ShowPaletteSelect(&g_interfaces.Player1.Char2(), "P1Ch2 palette", "select1-2");
	ShowPaletteSelect(&g_interfaces.Player2.Char1(), "P2Ch1 palette", "select2-1");
	ShowPaletteSelect(&g_interfaces.Player2.Char2(), "P2Ch2 palette", "select2-2");
}

void PaletteEditor::ShowReloadAllPalettesButton()
{
	if (ImGui::Button("Reload Custom Palettes"))
	{
		g_interfaces.pPaletteManager->ReloadPalettesFromFolder();

		//refresh the paletteVector variable
		customPaletteVector = g_interfaces.pPaletteManager->GetCustomPalettesVector();
	}
}

void PaletteEditor::OnMatchInit()
{
	if (HasNullPointer())
		return;

	InitializeSelectedCharacters();

	customPaletteVector = g_interfaces.pPaletteManager->GetCustomPalettesVector();

	selectedCharIndex = (CharIndex)allSelectedCharObjs[0]->char_index;
	selectedCharName = allSelectedCharNames[0];
	selectedCharPalInfo = allSelectedCharPalInfos[0];
	selectedPalIndex = 0;
	selectedFile = PaletteFile_Character;

	color_edit_flags = ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoAlpha;
	highlight_mode = false;
	show_alpha = false;

	char* fileAddr = g_interfaces.pPaletteManager->GetPalFileAddr(selectedFile, selectedCharPalInfo);
	CopyToEditorArray(fileAddr);

	g_gameVals.isPaletteModePaused = false;
}

bool PaletteEditor::HasNullPointer()
{
	if (g_interfaces.Player1.Char1().IsNullPtrCharData() ||
		g_interfaces.Player1.Char2().IsNullPtrCharData() ||
		g_interfaces.Player2.Char1().IsNullPtrCharData() ||
		g_interfaces.Player2.Char2().IsNullPtrCharData())
	{
		return true;
	}

	return false;
}

void PaletteEditor::InitializeSelectedCharacters()
{
	allSelectedCharObjs[0] = g_interfaces.Player1.Char1().Data();
	allSelectedCharObjs[1] = g_interfaces.Player1.Char2().Data();
	allSelectedCharObjs[2] = g_interfaces.Player2.Char1().Data();
	allSelectedCharObjs[3] = g_interfaces.Player2.Char2().Data();

	allSelectedCharNames[0] = charNames[allSelectedCharObjs[0]->char_index];
	allSelectedCharNames[1] = charNames[allSelectedCharObjs[1]->char_index];
	allSelectedCharNames[2] = charNames[allSelectedCharObjs[2]->char_index];
	allSelectedCharNames[3] = charNames[allSelectedCharObjs[3]->char_index];

	allSelectedCharPalInfos[0] = &g_interfaces.Player1.Char1().PalHandle();
	allSelectedCharPalInfos[1] = &g_interfaces.Player1.Char2().PalHandle();
	allSelectedCharPalInfos[2] = &g_interfaces.Player2.Char1().PalHandle();
	allSelectedCharPalInfos[3] = &g_interfaces.Player2.Char2().PalHandle();
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
		
		for (int i = 0; i < 4; i++)
		{
			ImGui::PushID(i);
			if (ImGui::Selectable(allSelectedCharNames[i]))
			{
				DisableHighlightModes();

				selectedCharIndex = (CharIndex)allSelectedCharObjs[i]->char_index;
				selectedCharName = allSelectedCharNames[i];
				selectedCharPalInfo = allSelectedCharPalInfos[i];
				selectedPalIndex = g_interfaces.pPaletteManager->GetCurrentCustomPalIndex(selectedCharPalInfo);

				char* fileAddr = g_interfaces.pPaletteManager->GetPalFileAddr(selectedFile, selectedCharPalInfo);
				CopyToEditorArray(fileAddr);

				ImGui::PopID();
			}
		}
		ImGui::EndPopup();
	}
}

void PaletteEditor::PaletteSelection()
{
	LOG(7, "PaletteEditor PaletteSelection\n");

	if (ImGui::Button("Select palette  "))
		ImGui::OpenPopup("select_custom_pal");

	//keep updating it, incase palette is changed on the menu
	selectedPalIndex = g_interfaces.pPaletteManager->GetCurrentCustomPalIndex(selectedCharPalInfo);

	ImGui::SameLine();
	ImGui::Text(customPaletteVector[selectedCharIndex][selectedPalIndex].palname);

	if (ImGui::BeginPopup("select_custom_pal"))
	{
		for (int i = 0; i < customPaletteVector[selectedCharIndex].size(); i++)
		{
			if (ImGui::Selectable(customPaletteVector[selectedCharIndex][i].palname))
			{
				DisableHighlightModes();

				selectedPalIndex = i;
				g_interfaces.pPaletteManager->SwitchPalette(selectedCharIndex, i, selectedCharPalInfo);
				char* fileAddr = g_interfaces.pPaletteManager->GetPalFileAddr(selectedFile, selectedCharPalInfo);
				CopyToEditorArray(fileAddr);
			}
			ShowHoveredPaletteToolTip(selectedCharIndex, i);
		}
		ImGui::EndPopup();
	}
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

				char* fileAddr = g_interfaces.pPaletteManager->GetPalFileAddr(selectedFile, selectedCharPalInfo);
				CopyToEditorArray(fileAddr);
			}
		}
		ImGui::EndPopup();
	}
}

void PaletteEditor::EditingModesSelection()
{
	LOG(7, "PaletteEditor EditingModesSelection\n");

	if (ImGui::Checkbox("Show transparency values", &show_alpha))
	{
		if (show_alpha)
			color_edit_flags = ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_AlphaBar;
		else
			color_edit_flags = ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoAlpha;
	}

	ImGui::SameLine();
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
			g_interfaces.pPaletteManager->ReplacePaletteFile(highlightArray, selectedFile, selectedCharPalInfo);
		}
		else
		{
			DisableHighlightModes();
		}
	}
}

void PaletteEditor::ShowPaletteBoxes()
{
	LOG(7, "PaletteEditor ShowPaletteBoxes\n");

	ImGui::Text("");
	ImGui::Separator();
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2, 2));

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
				g_interfaces.pPaletteManager->ReplacePaletteFile(paletteEditorArray, selectedFile, selectedCharPalInfo);
			}
		}

		if (col < COLUMN_LENGTH)
		{
			//continue the row
			ImGui::SameLine();
			col++;
		}
		else
		{
			//start a new row
			col = 1;
		}
		ImGui::PopID();
	}
	ImGui::PopStyleVar();
}

void PaletteEditor::DisableHighlightModes()
{
	highlight_mode = false;
	g_interfaces.pPaletteManager->ReplacePaletteFile(paletteEditorArray, selectedFile, selectedCharPalInfo);
}

void PaletteEditor::SavePaletteToFile()
{
	static char palNameBuf[IMPL_PALNAME_LENGTH] = "";
	static char palDescBuf[IMPL_DESC_LENGTH] = "";
	static char palCreatorBuf[IMPL_CREATOR_LENGTH] = "";
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

		TCHAR pathBuf[MAX_PATH];
		GetModuleFileName(NULL, pathBuf, MAX_PATH);
		std::wstring::size_type pos = std::wstring(pathBuf).find_last_of(L"\\");
		std::wstring fullPath = std::wstring(pathBuf).substr(0, pos);

		fullPath += L"\\BBTAG_IM\\Palettes\\";
		fullPath += wCharNames[selectedCharIndex];
		fullPath += L"\\";

		std::string filenameTemp(palNameBuf);
		std::wstring filename(filenameTemp.begin(), filenameTemp.end());
		fullPath += filename;

		if (filename.find(L".impl") == std::wstring::npos)
		{
			fullPath += L".impl";
			filenameTemp += ".impl";
		}

		std::string path(fullPath.begin(), fullPath.end());

		//overwrite popup code start
		if (PathFileExists(fullPath.c_str()))
		{
			ImGui::OpenPopup("Overwrite?");
			show_overwrite_popup = true;
		}
		if (ImGui::BeginPopupModal("Overwrite?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			static bool overwrite = false;
			ImGui::Text("'%s' already exists.\nAre you sure you want to overwrite it?\n\n", filenameTemp.c_str());
			ImGui::Separator();
			if (ImGui::Button("OK", ImVec2(120, 0)))
			{
				ImGui::CloseCurrentPopup();
				show_overwrite_popup = false;
				overwrite = true;
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(120, 0)))
			{
				ImGui::CloseCurrentPopup();
				show_overwrite_popup = false;
				overwrite = false;
			}
			ImGui::EndPopup();

			if (!overwrite)
				return;

			overwrite = false;
		}

		IMPL_data_t curPalData = g_interfaces.pPaletteManager->GetCurrentPalData(selectedCharPalInfo);

		strncpy(curPalData.creator, palCreatorBuf, IMPL_CREATOR_LENGTH);
		strncpy(curPalData.palname, palNameBuf, IMPL_PALNAME_LENGTH);
		strncpy(curPalData.desc, palDescBuf, IMPL_DESC_LENGTH);

		std::string messageText = "'";
		messageText += filenameTemp.c_str();

		if (g_interfaces.pPaletteManager->WritePaletteToFile(selectedCharIndex, &curPalData))
		{
			WindowManager::AddLog("[system] Custom palette '%s' successfully saved.\n", filenameTemp.c_str());	
			messageText += "' saved successfully";
		}
		else
		{ 
			WindowManager::AddLog("[system] Custom palette '%s' failed to be saved.\n", filenameTemp.c_str());
			messageText += "' save failed";
		}
		memcpy(message, messageText.c_str(), messageText.length() + 1);

		//TODO: load the newly saved palette into the customPaletteVector
	}
}

void PaletteEditor::CheckSelectedPalOutOfBound()
{
	if (selectedPalIndex != 0 && selectedPalIndex >= customPaletteVector[selectedCharIndex].size())
	{
		//reset back to default
		selectedPalIndex = 0;
		g_interfaces.pPaletteManager->SwitchPalette(selectedCharIndex, selectedPalIndex, selectedCharPalInfo);
		char* fileAddr = g_interfaces.pPaletteManager->GetPalFileAddr(selectedFile, selectedCharPalInfo);
		CopyToEditorArray(fileAddr);
	}
}

void PaletteEditor::ShowPaletteSelect(CharHandle * charHandle, const char * btnText, const char * popupID)
{
	CharPaletteHandle& charPalHandle = charHandle->PalHandle();
	int selected_pal_index = g_interfaces.pPaletteManager->GetCurrentCustomPalIndex(&charPalHandle);
	CharIndex charIndex = (CharIndex)charHandle->Data()->char_index;

	ImGui::Text(" "); ImGui::SameLine();
	if (ImGui::Button(btnText))
		ImGui::OpenPopup(popupID);

	ImGui::SameLine();
	ImGui::TextUnformatted(customPaletteVector[charIndex][selected_pal_index].palname);

	if (ImGui::BeginPopup(popupID))
	{
		ImGui::Text(charNames[charIndex]);
		ImGui::Separator();
		for (int i = 0; i < customPaletteVector[charIndex].size(); i++)
		{
			if (ImGui::Selectable(customPaletteVector[charIndex][i].palname))
			{
				g_interfaces.pPaletteManager->SwitchPalette(charIndex, i, &charPalHandle);

				//updating palette editor's array if this is the currently selected character
				if (&charPalHandle == selectedCharPalInfo)
				{
					DisableHighlightModes();

					char* fileAddr = g_interfaces.pPaletteManager->GetPalFileAddr(selectedFile, &charPalHandle);
					CopyToEditorArray(fileAddr);
				}
			}

			ShowHoveredPaletteToolTip(charIndex, i);
		}
		ImGui::EndPopup();
	}
}

void PaletteEditor::ShowHoveredPaletteToolTip(CharIndex charIndex, int palIndex)
{
	if (ImGui::IsItemHovered() && palIndex != 0)
	{
		char* creatorText = customPaletteVector[charIndex][palIndex].creator;
		char* descText = customPaletteVector[charIndex][palIndex].desc;
		int creatorLen = strnlen(creatorText, IMPL_CREATOR_LENGTH);
		int descLen = strnlen(descText, IMPL_DESC_LENGTH);

		if (creatorLen || descLen)
		{
			ImGui::BeginTooltip();

			if(creatorLen)
				ImGui::Text("Creator: %s", creatorText);
			if(descLen)
				ImGui::Text("Description: %s", descText);

			ImGui::EndTooltip();
		}
	}
}

void PaletteEditor::CopyToEditorArray(char * pSrc)
{
	memcpy(paletteEditorArray, pSrc, IMPL_PALETTE_DATALEN);
}

void PaletteEditor::UpdateHighlightArray(int selectedBoxIndex)
{
	static int selected_highlight_box = 0;

	//setting previously pressed box back to black
	((int*)highlightArray)[selected_highlight_box] = COLOR_BLACK;

	selected_highlight_box = selectedBoxIndex;

	//setting currently pressed box to white
	((int*)highlightArray)[selected_highlight_box] = COLOR_WHITE;

	g_interfaces.pPaletteManager->ReplacePaletteFile(highlightArray, selectedFile, selectedCharPalInfo);
}
