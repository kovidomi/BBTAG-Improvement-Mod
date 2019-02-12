#pragma once
#include "interfaces.h"
#include "CustomHud.h"
#include "PaletteEditor.h"

//forward declaration
struct ImGuiLog;

class WindowManager
{
private:
	static CustomHud* m_customHud;
	static PaletteEditor* m_paletteEditor;
	static ImGuiLog Log;
	static bool Initialized;
	static bool DoLogging;
public:
	static bool IsUpdateAvailable;

public:
	static bool Init(void *hwnd, IDirect3DDevice9 *device);
	static void OnUpdate();
	static void OnRender();
	static void Shutdown();
	static void InvalidateDeviceObjects();
	static void CreateDeviceObjects();
	static void OnMatchInit();
	// start message with one of these: "[system]", "[info]", "[warning]", "[error]", "[fatal]", "[notice]", "[log]"
	static void AddLog(const char* message, ...);
	static void AddLogSeparator();
	static void DisableLogging();
	static void EnableLogging();
	static void SetNotification(const char *text, float timeToShowInSec = 5.0, bool showNotificationWindow = false);
private:
	static void SetMainWindowTitle(const char *text = 0);
	static void HandleNotification();
	static void ShowNotificationWindow();
	static void ShowLogWindow(bool* p_open);
	static void ShowUpdateWindow();
	static void ShowLoadedSettingsValues();
	static void ShowDebugWindow(bool* p_open);
	static void ShowDonatorsWindow();
	static void ShowDonatorsButton();
	static void ShowMainWindow(bool* p_open);
	static void HandleMainWindowVisibility(float timeLeft);
	static void ShowLinks();
	static void ShowAllWindows();
	static void WriteLogToFile();
	static void HandleButtons();
};