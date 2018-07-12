#pragma once
#include "../interfaces.h"
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

public:
	static bool Initialized;
	static bool Init(void *hwnd, IDirect3DDevice9 *device);
	static void Update();
	static void Render();
	static void Shutdown();
	static void InvalidateDeviceObjects();
	static void CreateDeviceObjects();
	// start message with one of these: "[system]", "[info]", "[warning]", "[error]", "[fatal]", "[notice]", "[log]"
	static void AddLog(const char* message, ...);
	static void AddLogSeparator();
	static void SetNotification(const char *text, float timeToShowInSec = 5.0, bool showNotificationWindow = false);
	static bool IsUpdateAvailable;
	static bool DoLogging;
	static void OnMatchInit();
private:
	static void SetMainWindowTitle(const char *text = 0);
	static void HandleNotification();
	static void ShowNotificationWindow();
	static void ShowLogWindow(bool* p_open);
	static void ShowUpdateWindow();
	static void ShowLoadedIniSettings();
	static void ShowDebugWindow(bool* p_open);
	static void WriteLogToFile();
	static void HandleButtons();
};