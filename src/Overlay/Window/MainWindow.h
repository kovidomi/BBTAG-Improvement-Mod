#pragma once
#include "Window.h"
#include "DebugWindow.h"
#include "DonatorsWindow.h"
#include "LogWindow.h"
#include "UpdateNotifierWindow.h"
#include "PaletteEditorWindow.h"
#include "Overlay/CustomHud.h"
#include "Core/logger.h"

class MainWindow : public Window
{
public:
	MainWindow(const std::string& windowTitle, bool windowClosable,
		ImGuiWindowFlags windowFlags = 0)
		: Window(windowTitle, windowClosable, windowFlags)
	{
		m_middleScreen = ImVec2((float)Settings::settingsIni.renderwidth / 2, (float)Settings::settingsIni.renderheight / 2);

		//dividing by 1904x1042 because the custom HUD was designed on that resolution
		float hudScaleX = ((float)Settings::settingsIni.renderwidth * Settings::settingsIni.customhudscale) / 1904.0f;
		float hudScaleY = ((float)Settings::settingsIni.renderheight * Settings::settingsIni.customhudscale) / 1042.0f;
		LOG(2, "hud_scale_x: %f\n", hudScaleX);
		LOG(2, "hud_scale_y: %f\n", hudScaleY);
		m_customHud.SetScale(hudScaleX, hudScaleY);
		m_showCustomHud = Settings::settingsIni.forcecustomhud;
	}

	~MainWindow() override = default;
	void UpdateWindows();
	PaletteEditorWindow& getPaletteEditorWindow();
	LogWindow& getLogWindow();
	UpdateNotifierWindow& getUpdateNotifierWindow();

protected:
	void BeforeDraw() override;
	void Draw() override;
private:
	void DrawDonatorsButton(Window& donatorsWindow);
	void DrawSingleLinkButton(const std::string& label, const wchar_t* url) const;
	void DrawLinkButtons() const;
	void DrawLoadedSettingsValues() const;

	DebugWindow m_debugWindow = DebugWindow("DEBUG", true);
	DonatorsWindow m_donatorsWindow = DonatorsWindow("", false,
		ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);
	LogWindow m_logWindow = LogWindow("Log", true, ImGuiWindowFlags_NoCollapse);
	UpdateNotifierWindow m_updateNotifierWindow = UpdateNotifierWindow("Update available",
		true, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);
	PaletteEditorWindow m_paletteEditorWindow = PaletteEditorWindow("Palette Editor", true);
	CustomHud m_customHud = CustomHud();

	bool m_showCustomHud = false;
	ImVec2 m_middleScreen;
};
