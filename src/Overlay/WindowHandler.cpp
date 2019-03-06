#include "WindowHandler.h"

void WindowHandler::DrawAllWindows()
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

Window* WindowHandler::GetWindow(WindowType_ type)
{
	return m_windows[type];
}

void WindowHandler::InitWindowMap()
{
	m_windows[WindowType_Main] = &m_mainWindow;
	m_windows[WindowType_Debug] = &m_debugWindow;
	m_windows[WindowType_Log] = &m_logWindow;
	m_windows[WindowType_Donators] = &m_donatorsWindow;
	m_windows[WindowType_UpdateNotifier] = &m_updateNotifierWindow;
	m_windows[WindowType_PaletteEditor] = &m_paletteEditorWindow;
	//m_windows[WindowType_CustomHud] = &m_customHud;
}
