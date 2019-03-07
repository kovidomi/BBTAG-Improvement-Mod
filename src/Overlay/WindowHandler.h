#pragma once
#include "CustomHud.h"
#include "WindowContainer.h"

#include "Window/PaletteEditorWindow.h"
#include "Window/LogWindow.h"
#include "Window/UpdateNotifierWindow.h"
#include "Window/DonatorsWindow.h"
#include "Window/DebugWindow.h"
#include "Window/MainWindow.h"

#include "Core/info.h"
#include "Core/logger.h"
#include "Core/Settings.h"
#include "imgui.h"
#include <map>

class WindowHandler : public WindowContainer
{
public:
	WindowHandler()
	{
		//m_middleScreen = ImVec2((float)Settings::settingsIni.renderwidth / 2, (float)Settings::settingsIni.renderheight / 2);

		//dividing by 1904x1042 because the custom HUD was designed on that resolution
		float hudScaleX = ((float)Settings::settingsIni.renderwidth * Settings::settingsIni.customhudscale) / 1904.0f;
		float hudScaleY = ((float)Settings::settingsIni.renderheight * Settings::settingsIni.customhudscale) / 1042.0f;
		LOG(2, "hud_scale_x: %f\n", hudScaleX);
		LOG(2, "hud_scale_y: %f\n", hudScaleY);
		m_customHud.SetScale(hudScaleX, hudScaleY);
		m_showCustomHud = Settings::settingsIni.forcecustomhud;

		m_mainWindow.SetWindowHandler(*this);
		InitWindowMap();
	}

	~WindowHandler() override = default;
	void DrawAllWindows();
private:
	void InitWindowMap();

	MainWindow           m_mainWindow           = MainWindow(MOD_WINDOW_TITLE, false, ImGuiWindowFlags_AlwaysAutoResize);
	DebugWindow          m_debugWindow          = DebugWindow("DEBUG", true);
	DonatorsWindow       m_donatorsWindow       = DonatorsWindow("##DONATORS", false,
		ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);
	LogWindow            m_logWindow            = LogWindow("Log", true, ImGuiWindowFlags_NoCollapse);
	UpdateNotifierWindow m_updateNotifierWindow = UpdateNotifierWindow("Update available", true,
		ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);
	PaletteEditorWindow  m_paletteEditorWindow  = PaletteEditorWindow("Palette Editor", true);
	CustomHud            m_customHud            = CustomHud();

	bool m_showCustomHud = false;
	//ImVec2 m_middleScreen;
};
