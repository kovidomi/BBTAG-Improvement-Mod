#pragma once
#include "CustomHud.h"
#include "PaletteEditor.h"

#include <d3d9.h>

struct ImGuiLog;

class OverlayManager
{
	static OverlayManager* m_instance;
	CustomHud* m_customHud = nullptr;
	PaletteEditor* m_paletteEditor = nullptr;
	static ImGuiLog Log;
	bool Initialized = false;
	bool DoLogging = true;
public:
	bool IsUpdateAvailable = false;

	static OverlayManager& getInstance();

	bool Init(void *hwnd, IDirect3DDevice9 *device);
	void OnUpdate();
	void OnRender();
	void Shutdown();
	void InvalidateDeviceObjects();
	void CreateDeviceObjects();
	void OnMatchInit();
	// start message with one of these: "[system]", "[info]", "[warning]", "[error]", "[fatal]", "[notice]", "[log]"
	void AddLog(const char* message, ...);
	void AddLogSeparator();
	void DisableLogging();
	void EnableLogging();
	void SetNotification(const char *text, float timeToShowInSec = 5.0, bool showNotificationWindow = false);
private:
	OverlayManager();
	void SetMainWindowTitle(const char *text = 0);
	void HandleNotification();
	void ShowNotificationWindow();
	void ShowLogWindow(bool* p_open);
	void ShowUpdateWindow();
	void ShowLoadedSettingsValues();
	void ShowDebugWindow(bool* p_open);
	void ShowDonatorsWindow();
	void ShowDonatorsButton();
	void ShowMainWindow(bool* p_open);
	void HandleMainWindowVisibility(float timeLeft);
	void ShowLinks();
	void ShowAllWindows();
	void WriteLogToFile();
	void HandleButtons();
};