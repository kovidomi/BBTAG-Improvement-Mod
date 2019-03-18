#pragma once
#include "WindowContainer/WindowContainer.h"

#include <d3d9.h>

class WindowManager
{
public:
	static WindowManager& getInstance();
	bool Init(void *hwnd, IDirect3DDevice9 *device);
	void Update();
	void Render();
	void Shutdown();
	void InvalidateDeviceObjects();
	void CreateDeviceObjects();
	void OnMatchInit();
	bool IsInitialized() const;

	// start message with one of these: "[system]", "[info]", "[warning]", "[error]", "[fatal]", "[notice]", "[log]"
	void AddLog(const char* message, ...);
	void AddLogSeparator();
	void SetLogging(bool value);
	void SetNotification(const char *text, float timeToShowInSec = 5.0, bool showNotificationWindow = false);
	void SetUpdateAvailable();
private:
	WindowManager() = default;
	void WriteLogToFile();
	void HandleButtons();

	static WindowManager* m_instance;
	bool m_initialized = false;
	WindowContainer* m_windowContainer = nullptr;
};
