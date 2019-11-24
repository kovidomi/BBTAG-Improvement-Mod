#include "MainWindow.h"

#include "HitboxOverlay.h"
#include "PaletteEditorWindow.h"
#include "CustomHud/CustomHudWindow.h"

#include "Core/Settings.h"
#include "Core/info.h"
#include "Core/interfaces.h"
#include "Game/gamestates.h"
#include "Overlay/imgui_utils.h"
#include "Web/donators_fetch.h"

void MainWindow::SetMainWindowTitle(const std::string text)
{
	if (text != "")
	{
		m_windowTitle = text;
	}
	else
	{
		m_windowTitle = MOD_WINDOW_TITLE;
		m_windowTitle += " ";
		m_windowTitle += MOD_VERSION_NUM;
#ifdef _DEBUG
		m_windowTitle += " (DEBUG)";
#endif
	}
	m_windowTitle += "###MainTitle"; //set unique identifier
}

void MainWindow::SetWindowContainer(WindowContainer & windowContainer)
{
	m_pWindowContainer = &windowContainer;
}


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
	ImGui::Text("Toggle me with %s", Settings::settingsIni.togglebutton.c_str());
	ImGui::Text("Toggle HUD with %s", Settings::settingsIni.toggleHUDbutton.c_str());
	ImGui::Text("Toggle Custom HUD with %s", Settings::settingsIni.togglecustomHUDbutton.c_str());
	ImGui::Separator();

	DrawDonatorsButton();

	ImGui::Text("");

	DrawCustomHudSection();

	DrawCustomPalettesSection();

	DrawHitboxOverlaySection();

	if (ImGui::CollapsingHeader("Loaded settings.ini values"))
	{
		DrawLoadedSettingsValues();
	}

#ifdef _DEBUG
	if (ImGui::Button("DEBUG"))
	{
		m_pWindowContainer->GetWindow(WindowType_Debug)->Open();
	}
#endif
	if (ImGui::Button("Log"))
	{
		m_pWindowContainer->GetWindow(WindowType_Log)->Open();
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

void MainWindow::DrawDonatorsButton()
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
		m_pWindowContainer->GetWindow(WindowType_Donators)->Open();
	}
}

void MainWindow::DrawCustomHudSection() const
{
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
		m_pWindowContainer->GetWindow<CustomHudWindow>(WindowType_CustomHud)->DrawShowCustomHudWindowCheckbox();

		ImGui::Text(" "); ImGui::SameLine();
		m_pWindowContainer->GetWindow<CustomHudWindow>(WindowType_CustomHud)->DrawSetWindowsMovableCheckbox();

		ImGui::Text(" "); ImGui::SameLine();
		m_pWindowContainer->GetWindow<CustomHudWindow>(WindowType_CustomHud)->DrawResetWindowsPositionsButton();
	}
}

void MainWindow::DrawCustomPalettesSection() const
{
	if (ImGui::CollapsingHeader("Custom palettes"))
	{
		if (*g_gameVals.pGameState != GameState_Match)
		{
			ImGui::Text(" "); ImGui::SameLine();
			ImGui::TextDisabled("Not in match!");
		}
		else
		{
			m_pWindowContainer->GetWindow<PaletteEditorWindow>(WindowType_PaletteEditor)->ShowAllPaletteSelections();
		}

		ImGui::Text(""); ImGui::Text(" "); ImGui::SameLine();
		m_pWindowContainer->GetWindow<PaletteEditorWindow>(WindowType_PaletteEditor)->ShowReloadAllPalettesButton();

		ImGui::Text(" "); ImGui::SameLine();
		bool pressed = ImGui::Button("Palette editor");

		if (!isPaletteEditingEnabledInCurrentGameMode())
		{
			ImGui::SameLine(); ImGui::TextDisabled("Not in training or versus modes!");
		}
		else if (isPaletteEditingEnabledInCurrentGameMode() && pressed)
		{
			m_pWindowContainer->GetWindow(WindowType_PaletteEditor)->Open();
		}
	}
}

void MainWindow::DrawHitboxOverlaySection() const
{
	if (ImGui::CollapsingHeader("Hitbox overlay"))
	{
		if (*g_gameVals.pGameState != GameState_Match)
		{
			ImGui::TextUnformatted(" "); ImGui::SameLine();
			ImGui::TextDisabled("Not in match!");
			return;
		}

		if (*g_gameVals.pGameMode != GameMode_Training)
		{
			ImGui::TextUnformatted(" "); ImGui::SameLine();
			ImGui::TextDisabled("Not in training mode!");
			return;
		}

		ImGui::TextUnformatted(" "); ImGui::SameLine();
		static bool isOpen = false;
		if (ImGui::Checkbox("Enable", &isOpen))
		{
			if (isOpen)
			{
				m_pWindowContainer->GetWindow(WindowType_HitboxOverlay)->Open();
			}
			else
			{
				m_pWindowContainer->GetWindow(WindowType_HitboxOverlay)->Close();
			}
		}
		ImGui::SameLine(); ImGui::TextUnformatted(" "); ImGui::SameLine();
		ImGui::Checkbox("Freeze frame", &g_gameVals.isFrameFrozen);

		ImGui::TextUnformatted(" "); ImGui::SameLine();
		ImGui::Checkbox("P1Ch1", &m_pWindowContainer->GetWindow<HitboxOverlay>(WindowType_HitboxOverlay)->drawCharacterHitbox[0]);
		HoverTooltip(getCharacterNameByIndexA(g_interfaces.player1.GetChar1().GetData()->char_index).c_str());

		ImGui::SameLine(); ImGui::TextUnformatted(" "); ImGui::SameLine();
		ImGui::Checkbox("P2Ch1", &m_pWindowContainer->GetWindow<HitboxOverlay>(WindowType_HitboxOverlay)->drawCharacterHitbox[1]);
		HoverTooltip(getCharacterNameByIndexA(g_interfaces.player2.GetChar1().GetData()->char_index).c_str());

		ImGui::TextUnformatted(" "); ImGui::SameLine();
		ImGui::Checkbox("P1Ch2", &m_pWindowContainer->GetWindow<HitboxOverlay>(WindowType_HitboxOverlay)->drawCharacterHitbox[2]);
		HoverTooltip(getCharacterNameByIndexA(g_interfaces.player1.GetChar2().GetData()->char_index).c_str());

		ImGui::SameLine(); ImGui::TextUnformatted(" "); ImGui::SameLine();
		ImGui::Checkbox("P2Ch2", &m_pWindowContainer->GetWindow<HitboxOverlay>(WindowType_HitboxOverlay)->drawCharacterHitbox[3]);
		HoverTooltip(getCharacterNameByIndexA(g_interfaces.player2.GetChar2().GetData()->char_index).c_str());

		ImGui::TextUnformatted(" "); ImGui::SameLine();
		m_pWindowContainer->GetWindow<HitboxOverlay>(WindowType_HitboxOverlay)->DrawRectThicknessSlider();

		ImGui::TextUnformatted(" "); ImGui::SameLine();
		m_pWindowContainer->GetWindow<HitboxOverlay>(WindowType_HitboxOverlay)->DrawRectFillTransparencySlider();

		ImGui::TextUnformatted(" "); ImGui::SameLine();
		ImGui::Checkbox("Draw origin",
			&m_pWindowContainer->GetWindow<HitboxOverlay>(WindowType_HitboxOverlay)->drawOriginLine);
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
