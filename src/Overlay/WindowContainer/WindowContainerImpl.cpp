#include "WindowContainerImpl.h"

#include "Overlay/Window/CustomHud/CustomHudWindow.h"
#include "Overlay/Window/DebugWindow.h"
#include "Overlay/Window/DonatorsWindow.h"
#include "Overlay/Window/LogWindow.h"
#include "Overlay/Window/MainWindow.h"
#include "Overlay/Window/PaletteEditorWindow.h"
#include "Overlay/Window/UpdateNotifierWindow.h"

#include "Core/info.h"
#include "Core/logger.h"
#include "Core/Settings.h"

void WindowContainerImpl::FillWindowContainer()
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

	CustomHudWindow* pCustomHud = new CustomHudWindow("Custom Hud", false,
		ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);

	//dividing by 1904x1042 because the custom HUD was designed on that resolution
	ImVec2 scale = ImVec2(
		(Settings::settingsIni.renderwidth * Settings::settingsIni.customhudscale) / 1904.0f,
		(Settings::settingsIni.renderheight * Settings::settingsIni.customhudscale) / 1042.0f
	);
	LOG(2, "Overlay scale: x: %.2f y: %.2f\n", scale.x, scale.y);
	pCustomHud->SetScale(scale);
	AddWindow(WindowType_CustomHud, pCustomHud);
}
