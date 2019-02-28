#pragma once
#include "CustomHud.h"

#include <d3d9.h>

class OverlayManager
{
public:
	static OverlayManager& getInstance();
	bool Init(void *hwnd, IDirect3DDevice9 *device);
	void OnUpdate();
	void OnRender();
	void Shutdown();
	void InvalidateDeviceObjects();
	void CreateDeviceObjects();
	void OnMatchInit();
	bool IsInitialized() const;

	// start message with one of these: "[system]", "[info]", "[warning]", "[error]", "[fatal]", "[notice]", "[log]"
	void AddLog(const char* message, ...);
	void AddLogSeparator();
	void SetLogging(bool value);
	// void SetNotification(const char *text, float timeToShowInSec = 5.0, bool showNotificationWindow = false);
	void SetUpdateAvailable();

private:
	OverlayManager();
	void SetMainWindowTitle(const char *text = 0);
	void HandleNotification();
	// void ShowNotificationWindow();
	// void ShowLoadedSettingsValues();
	// void ShowDonatorsButton();
	// void ShowMainWindow(bool* p_open);
	void HandleMainWindowVisibility(float timeLeft);
	// void ShowLinks();
	void ShowAllWindows();
	void WriteLogToFile();
	void HandleButtons();

	static OverlayManager* m_instance;
	CustomHud*             m_customHud = nullptr;
	bool                   m_initialized = false;
};