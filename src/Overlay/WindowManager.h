#pragma once
#include "WindowContainer/WindowContainer.h"
#include "Logger/Logger.h"

#include <d3d9.h>

class WindowManager
{
public:
	static WindowManager& GetInstance();
	bool Init(void *hwnd, IDirect3DDevice9 *device);
	void Update();
	void Render();
	void Shutdown();
	void InvalidateDeviceObjects();
	void CreateDeviceObjects();
	bool IsInitialized() const;
	WindowContainer* GetWindowContainer() const;
	void OnMatchInit();

private:
	WindowManager() = default;
	void WriteLogToFile();
	void HandleButtons();

	static WindowManager* m_instance;
	bool m_initialized = false;
	WindowContainer* m_windowContainer = nullptr;
	Logger* m_pLogger = nullptr;
};
