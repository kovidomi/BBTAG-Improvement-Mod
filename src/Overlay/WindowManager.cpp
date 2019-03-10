#include "WindowManager.h"
#include "Core/logger.h"
#include "Core/info.h"
#include "Core/Settings.h"
#include "Window/DebugWindow.h"
#include "Window/DonatorsWindow.h"
#include "Window/LogWindow.h"
#include "Window/MainWindow.h"
#include "Window/PaletteEditorWindow.h"
#include "Window/UpdateNotifierWindow.h"

WindowManager::WindowManager()
{
	InitWindows();
	//m_middleScreen = ImVec2((float)Settings::settingsIni.renderwidth / 2, (float)Settings::settingsIni.renderheight / 2);

	//dividing by 1904x1042 because the custom HUD was designed on that resolution
	float hudScaleX = ((float)Settings::settingsIni.renderwidth * Settings::settingsIni.customhudscale) / 1904.0f;
	float hudScaleY = ((float)Settings::settingsIni.renderheight * Settings::settingsIni.customhudscale) / 1042.0f;
	LOG(2, "hud_scale_x: %f\n", hudScaleX);
	LOG(2, "hud_scale_y: %f\n", hudScaleY);
	m_customHud.SetScale(hudScaleX, hudScaleY);
	m_showCustomHud = Settings::settingsIni.forcecustomhud;

	GetWindow<MainWindow>(WindowType_Main)->SetWindowHandler(*this);
}

void WindowManager::DrawAllWindows()
{
	for (const auto& window : m_windows)
	{
		window.second->Update();
	}

	if (m_showCustomHud)
	{
		m_customHud.OnUpdate(m_showCustomHud, GetWindow(WindowType_Main)->IsOpen());
	}
}

void WindowManager::InitWindows()
{
	AddWindow(WindowType_Main,
		new MainWindow(MOD_WINDOW_TITLE, false,
			ImGuiWindowFlags_AlwaysAutoResize));

	AddWindow(WindowType_Debug,
		new DebugWindow("DEBUG", true));

	AddWindow(WindowType_Log,
		new LogWindow("Log", true,
			ImGuiWindowFlags_NoCollapse));

	AddWindow(WindowType_Donators,
		new DonatorsWindow("##DONATORS", false,
			ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse));

	AddWindow(WindowType_UpdateNotifier,
		new UpdateNotifierWindow("Update available", true,
			ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse));

	AddWindow(WindowType_PaletteEditor,
		new PaletteEditorWindow("Palette Editor", true));

	//m_windows[WindowType_CustomHud] = &m_customHud;
}
