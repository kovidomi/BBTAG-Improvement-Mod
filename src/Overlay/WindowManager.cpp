#include "WindowManager.h"

void WindowManager::DrawAllWindows()
{
	for (const auto& window : m_windows)
	{
		window.second->Update();
	}

	if (m_showCustomHud)
	{
		m_customHud.OnUpdate(m_showCustomHud, m_mainWindow.IsOpen());
	}
}

void WindowManager::InitWindowMap()
{
	AddWindow(WindowType_Main, &m_mainWindow);
	AddWindow(WindowType_Main, &m_mainWindow);
	AddWindow(WindowType_Debug, &m_debugWindow);
	AddWindow(WindowType_Log, &m_logWindow);
	AddWindow(WindowType_Donators, &m_donatorsWindow);
	AddWindow(WindowType_UpdateNotifier, &m_updateNotifierWindow);
	AddWindow(WindowType_PaletteEditor, &m_paletteEditorWindow);
	//m_windows[WindowType_CustomHud] = &m_customHud;
}
