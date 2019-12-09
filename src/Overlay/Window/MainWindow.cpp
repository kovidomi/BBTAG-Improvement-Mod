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

#include <sstream>

#define SECTION_INDENT_WIDTH 3.0f

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
			windowSizeConstraints = ImVec2(350, 230);
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
	VerticalSpacing(10.0f);
	DrawCustomHudSection();
	DrawCustomPalettesSection();
	DrawHitboxOverlaySection();

	if (ImGui::CollapsingHeader("Loaded settings.ini values"))
	{
		DrawLoadedSettingsValues();
	}

#ifdef _DEBUG
	if (ImGui::Button("DEBUG", BTN_SIZE))
	{
		m_pWindowContainer->GetWindow(WindowType_Debug)->ToggleOpen();
	}
#endif
	if (ImGui::Button("Log", BTN_SIZE))
	{
		m_pWindowContainer->GetWindow(WindowType_Log)->ToggleOpen();
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
	if (ImGui::Button(buf, ImVec2(-1.0f, 24)))
	{
		m_pWindowContainer->GetWindow(WindowType_Donators)->ToggleOpen();
	}

	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::TextUnformatted("Donators");
		ImGui::EndTooltip();
	}
}

void MainWindow::DrawCustomHudSection() const
{
	if (ImGui::CollapsingHeader("Custom HUD"))
	{
		ImGui::Indent(SECTION_INDENT_WIDTH);
		ImGui::BeginChild("customhud_settings", ImVec2(0, 100), true);

		if (g_gameVals.pIsHUDHidden)
		{
			ImGui::Checkbox("Show HUD", (bool*)g_gameVals.pIsHUDHidden);
			if (Settings::settingsIni.forcecustomhud)
			{
				ImGui::SameLine();
				ImGui::TextDisabled("(ForceCustomHUD is ON)");
			}
		}

		m_pWindowContainer->GetWindow<CustomHudWindow>(WindowType_CustomHud)->DrawShowCustomHudWindowCheckbox();
		m_pWindowContainer->GetWindow<CustomHudWindow>(WindowType_CustomHud)->DrawSetWindowsMovableCheckbox();
		m_pWindowContainer->GetWindow<CustomHudWindow>(WindowType_CustomHud)->DrawResetWindowsPositionsButton();

		ImGui::EndChild();
		ImGui::Unindent(SECTION_INDENT_WIDTH);
	}
}

void MainWindow::DrawCustomPalettesSection() const
{
	if (ImGui::CollapsingHeader("Custom palettes"))
	{
		ImGui::Indent(SECTION_INDENT_WIDTH);
		ImGui::BeginChild("palette_settings", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);

		if (*g_gameVals.pGameState != GameState_Match)
		{
			ImGui::Indent(SECTION_INDENT_WIDTH);
			ImGui::TextDisabled("Not in match!");
			ImGui::Unindent(SECTION_INDENT_WIDTH);
		}
		else
		{
			m_pWindowContainer->GetWindow<PaletteEditorWindow>(WindowType_PaletteEditor)->ShowAllPaletteSelections();
		}

		VerticalSpacing(15.0f);
		m_pWindowContainer->GetWindow<PaletteEditorWindow>(WindowType_PaletteEditor)->ShowReloadAllPalettesButton();

		bool pressed = ImGui::Button("Palette editor");

		if (!isPaletteEditingEnabledInCurrentGameMode())
		{
			ImGui::SameLine();
			ImGui::TextDisabled("Not in training or versus modes!");
		}
		else if (isPaletteEditingEnabledInCurrentGameMode() && pressed)
		{
			m_pWindowContainer->GetWindow(WindowType_PaletteEditor)->ToggleOpen();
		}

		ImGui::EndChild();
		ImGui::Unindent(SECTION_INDENT_WIDTH);
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

		if (!isHitboxOverlayEnabledInCurrentGameMode())
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
				g_gameVals.isFrameFrozen = false;
				m_pWindowContainer->GetWindow(WindowType_HitboxOverlay)->Close();
			}
		}

		if (!isOpen)
		{
			return;
		}
		ImGui::Indent(SECTION_INDENT_WIDTH);
		ImGui::BeginChild("hitboxoverlay_settings", ImVec2(0, 200), true);

		if (g_interfaces.player1.GetChar1().GetData()
			&& g_interfaces.player2.GetChar1().GetData()
			&& g_interfaces.player1.GetChar2().GetData()
			&& g_interfaces.player2.GetChar2().GetData())
		{
			ImGui::Checkbox("P1Ch1", &m_pWindowContainer->GetWindow<HitboxOverlay>(WindowType_HitboxOverlay)->drawCharacterHitbox[0]);
			HoverTooltip(getCharacterNameByIndexA(g_interfaces.player1.GetChar1().GetData()->char_index).c_str());

			ImGui::SameLine();
			ImGui::Checkbox("P2Ch1", &m_pWindowContainer->GetWindow<HitboxOverlay>(WindowType_HitboxOverlay)->drawCharacterHitbox[1]);
			HoverTooltip(getCharacterNameByIndexA(g_interfaces.player2.GetChar1().GetData()->char_index).c_str());

			VerticalSpacing(2);

			ImGui::Checkbox("P1Ch2", &m_pWindowContainer->GetWindow<HitboxOverlay>(WindowType_HitboxOverlay)->drawCharacterHitbox[2]);
			HoverTooltip(getCharacterNameByIndexA(g_interfaces.player1.GetChar2().GetData()->char_index).c_str());

			ImGui::SameLine();
			ImGui::Checkbox("P2Ch2", &m_pWindowContainer->GetWindow<HitboxOverlay>(WindowType_HitboxOverlay)->drawCharacterHitbox[3]);
			HoverTooltip(getCharacterNameByIndexA(g_interfaces.player2.GetChar2().GetData()->char_index).c_str());
		}

		VerticalSpacing(10);

		m_pWindowContainer->GetWindow<HitboxOverlay>(WindowType_HitboxOverlay)->DrawRectThicknessSlider();
		m_pWindowContainer->GetWindow<HitboxOverlay>(WindowType_HitboxOverlay)->DrawRectFillTransparencySlider();

		ImGui::Checkbox("Draw origin",
			&m_pWindowContainer->GetWindow<HitboxOverlay>(WindowType_HitboxOverlay)->drawOriginLine);

		VerticalSpacing(10);

		ImGui::Checkbox("Freeze frame:", &g_gameVals.isFrameFrozen);
		if (g_gameVals.pFrameCount)
		{
			ImGui::SameLine();
			ImGui::Text("%d", *g_gameVals.pFrameCount);
			ImGui::SameLine();
			if (ImGui::Button("Reset"))
			{
				*g_gameVals.pFrameCount = 0;
				g_gameVals.framesToReach = 0;
			}
		}

		if (g_gameVals.isFrameFrozen)
		{
			static int framesToStep = 1;
			if (ImGui::Button("Step frames"))
			{
				g_gameVals.framesToReach = *g_gameVals.pFrameCount + framesToStep;
			}

			ImGui::SameLine();
			ImGui::SliderInt("", &framesToStep, 1, 60);
		}

		ImGui::EndChild();
		ImGui::Unindent(SECTION_INDENT_WIDTH);
	}
}

void MainWindow::DrawLinkButtons() const
{
	ButtonUrl("Discord", MOD_LINK_DISCORD, BTN_SIZE);

	ImGui::SameLine();
	ButtonUrl("Forum", MOD_LINK_FORUM, BTN_SIZE);

	ImGui::SameLine();
	ButtonUrl("GitHub", MOD_LINK_GITHUB, BTN_SIZE);

	ImGui::SameLine();
	ButtonUrl("Donate", MOD_LINK_DONATE, BTN_SIZE);
}

void MainWindow::DrawLoadedSettingsValues() const
{
	ImGui::Indent(SECTION_INDENT_WIDTH);
	ImGui::BeginChild("loaded_settings", ImVec2(0, 300.0f), true, ImGuiWindowFlags_HorizontalScrollbar);

	std::ostringstream oss;
	//X-Macro
#define SETTING(_type, _var, _inistring, _defaultval) \
	oss << _inistring; \
	ImGui::TextUnformatted(oss.str().c_str()); ImGui::SameLine(ImGui::GetWindowWidth() * 0.6f); \
	oss.str(""); \
	oss << "= " << Settings::settingsIni.##_var; \
	ImGui::TextUnformatted(oss.str().c_str()); ImGui::Separator(); \
	oss.str("");
#include "Core/settings.def"
#undef SETTING

	ImGui::EndChild();
	ImGui::Unindent(SECTION_INDENT_WIDTH);
}
