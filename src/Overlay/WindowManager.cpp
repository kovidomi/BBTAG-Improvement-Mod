#include "WindowManager.h"

#include "Window/CustomHud/CustomHudWindow.h"
#include "Window/DebugWindow.h"
#include "Window/DonatorsWindow.h"
#include "Window/LogWindow.h"
#include "Window/MainWindow.h"
#include "Window/PaletteEditorWindow.h"
#include "Window/UpdateNotifierWindow.h"

#include "Core/info.h"
#include "Core/logger.h"
#include "Core/Settings.h"

void WindowManager::FillWindowContainer()
{
	MainWindow* pMainWindow = new MainWindow(MOD_WINDOW_TITLE, false, ImGuiWindowFlags_AlwaysAutoResize);
	pMainWindow->SetWindowContainer(*this);
	AddWindow(WindowType_Main, pMainWindow);

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

	CustomHudWindow* pCustomHud = new CustomHudWindow("Custom Hud", false, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);
	//dividing by 1904x1042 because the custom HUD was designed on that resolution
	ImVec2 scale = ImVec2(
		(Settings::settingsIni.renderwidth * Settings::settingsIni.customhudscale) / 1904.0f,
		(Settings::settingsIni.renderheight * Settings::settingsIni.customhudscale) / 1042.0f
	);
	LOG(2, "Overlay scale: x: %.2f y: %.2f\n", scale.x, scale.y);
	pCustomHud->SetScale(scale);
	AddWindow(WindowType_CustomHud, pCustomHud);
}
