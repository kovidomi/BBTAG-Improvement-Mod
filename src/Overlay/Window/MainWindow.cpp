#include "MainWindow.h"
#include "Core/Settings.h"
#include "Core/info.h"

void MainWindow::BeforeDraw()
{
	ImGui::SetWindowPos(m_windowTitle.c_str(), ImVec2(12, 20), ImGuiCond_FirstUseEver);

	ImVec2 windowSizeConstraints;
	switch (Settings::settingsIni.menusize)
	{
		case 1:
			windowSizeConstraints = ImVec2(250, 190);
			break;
		case 3:
			windowSizeConstraints = ImVec2(400, 230);
			break;
		default:
			windowSizeConstraints = ImVec2(330, 230);
	}

	ImGui::SetNextWindowSizeConstraints(windowSizeConstraints, ImVec2(1000, 1000));
}

void MainWindow::Draw()
{
	//ImGui::Begin(main_title.c_str(), NO_CLOSE_FLAG, ImGuiWindowFlags_AlwaysAutoResize);

	// prevent disappearing if clicked on
	if (main_window_disappear_time > 0)
	{
		if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(0))
		{
			HandleMainWindowVisibility(0);
		}
	}

	ImGui::Text("Toggle me with %s", Settings::settingsIni.togglebutton.c_str());
	ImGui::Text("Toggle HUD with %s", Settings::settingsIni.toggleHUDbutton.c_str());
	ImGui::Text("Toggle Custom HUD with %s", Settings::settingsIni.togglecustomHUDbutton.c_str());
	ImGui::Separator();

	ShowDonatorsButton();

	ImGui::Text("");

	if (ImGui::CollapsingHeader("Custom HUD"))
	{
		if (g_gameVals.pIsHUDHidden)
		{
			ImGui::Text(" "); ImGui::SameLine();
			ImGui::Checkbox("Show HUD", (bool*)g_gameVals.pIsHUDHidden);
			if (Settings::settingsIni.forcecustomhud)
			{
				ImGui::SameLine(); ImGui::TextDisabled("(ForceCustomHUD is ON)");
			}
		}

		ImGui::Text(" "); ImGui::SameLine();
		ImGui::Checkbox("Show Custom HUD", &show_custom_hud);

		ImGui::Text(" "); ImGui::SameLine();
		m_customHud->ShowResetPositionsButton(middlescreen);
	}

	if (ImGui::CollapsingHeader("Custom palettes"))
	{
		if (*g_gameVals.pGameState != GameState_Match)
		{
			ImGui::Text(" "); ImGui::SameLine();
			ImGui::TextDisabled("Not in match!");
		}
		else
		{
			g_paletteEditorWindow->ShowAllPaletteSelections();
		}

		ImGui::Text(""); ImGui::Text(" "); ImGui::SameLine();
		g_paletteEditorWindow->ShowReloadAllPalettesButton();

		ImGui::Text(" "); ImGui::SameLine();
		bool pressed = ImGui::Button("Palette editor");

		if (*g_gameVals.pGameMode != GameMode_Training)
		{
			ImGui::SameLine(); ImGui::TextDisabled("Not in training mode!");
		}
		else if (*g_gameVals.pGameMode == GameMode_Training && pressed)
		{
			g_paletteEditorWindow->Open();
		}
	}

	if (ImGui::CollapsingHeader("Loaded settings.ini values"))
	{
		DrawLoadedSettingsValues();
	}

#ifdef _DEBUG
	if (ImGui::Button("Demo"))
		show_demo_window ^= 1;

	if (ImGui::Button("DEBUG"))
	{
		g_debugWindow->Open();
	}
#endif
	if (ImGui::Button("Log"))
	{
		g_logWindow->Open();
	}

	ImGui::Text("Current online players:"); ImGui::SameLine();
	if (g_interfaces.pSteamApiHelper)
	{
		ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "%s",
			g_interfaces.pSteamApiHelper->current_players <= 0 ? "<No data>" : std::to_string(g_interfaces.pSteamApiHelper->current_players).c_str());
	}
	else
	{
		ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "%s", "<No data>");
	}

	DrawLinkButtons();
}

void MainWindow::DrawDonatorsButton(Window & donatorsWindow)
{
	if (GetDonatorNames().empty())
	{
		return;
	}

	const float SPEED = 2.0f;
	int passedTime = (int)(ImGui::GetTime() / SPEED);
	static int prevPassedTime = 0;
	int donatorSize = GetDonatorNames().size() - 1;
	static int index = 0;

	if (passedTime > prevPassedTime)
	{
		prevPassedTime = passedTime;
		index++;
		if (index > donatorSize)
			index = 0;
	}

	std::string donatorName = "";

	if (index == 0)
		donatorName += "Top Donator: ";

	donatorName += GetDonatorNames()[index];

	char buf[128];
	sprintf(buf, "%s", donatorName.c_str());
	if (ImGui::Button(buf, ImVec2(-1.0f, 0.0f)))
	{
		donatorsWindow.Open();
	}
}

void MainWindow::DrawSingleLinkButton(const std::string& label, const wchar_t* url) const
{
	if (ImGui::Button(label.c_str()))
	{
		ShellExecute(nullptr, L"open", url, nullptr, nullptr, SW_SHOWNORMAL);
	}
}

void MainWindow::DrawLinkButtons() const
{
	DrawSingleLinkButton("Discord", MOD_LINK_DISCORD);

	ImGui::SameLine();
	DrawSingleLinkButton("Forum", MOD_LINK_FORUM);

	ImGui::SameLine();
	DrawSingleLinkButton("Nexusmods", MOD_LINK_NEXUSMODS);

	ImGui::SameLine();
	DrawSingleLinkButton("GitHub", MOD_LINK_GITHUB);

	ImGui::SameLine();
	DrawSingleLinkButton("Donate", MOD_LINK_DONATE);
}

void MainWindow::DrawLoadedSettingsValues() const
{
	//not using ImGui columns here because they are bugged if the window has always_autoresize flag. The window 
	//starts extending to infinity, if the left edge of the window touches any edges of the screen

	//TODO: Put the strings into the X-Macro as well. Somehow...
	//strings cant be put into the X-Macro (.c_str() cannot be put on non-std::string types)
	ImGui::Separator();
	ImGui::Text(" ToggleButton"); ImGui::SameLine(ImGui::GetWindowWidth() * 0.5f);
	ImGui::Text("= %s", Settings::settingsIni.togglebutton.c_str());
	ImGui::Separator();

	ImGui::Text(" ToggleHUDButton"); ImGui::SameLine(ImGui::GetWindowWidth() * 0.5f);
	ImGui::Text("= %s", Settings::settingsIni.toggleHUDbutton.c_str());
	ImGui::Separator();

	ImGui::Text(" ToggleCustomHUDButton"); ImGui::SameLine(ImGui::GetWindowWidth() * 0.5f);
	ImGui::Text("= %s", Settings::settingsIni.togglecustomHUDbutton.c_str());
	ImGui::Separator();

	std::string printText = "";

	//X-Macro
#define SETTING(_type, _var, _inistring, _defaultval) \
	if(strcmp(#_type, "std::string") != 0) { \
	printText = " "; \
	printText += _inistring; \
	ImGui::Text(printText.c_str()); ImGui::SameLine(ImGui::GetWindowWidth() * 0.5f); \
	if(strcmp(#_type, "bool") == 0 || strcmp(#_type, "int") == 0) \
		printText = "= %d"; \
	else if(strcmp(#_type, "float") == 0) \
		printText = "= %.2f"; \
	ImGui::Text(printText.c_str(), Settings::settingsIni.##_var); ImGui::Separator(); }
#include "Core/settings.def"
#undef SETTING
}
