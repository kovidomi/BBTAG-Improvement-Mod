#pragma once
#include "Window/Window.h"
#include <map>

enum WindowType_
{
	WindowType_Main,
	WindowType_Debug,
	WindowType_Log,
	WindowType_Donators,
	WindowType_UpdateNotifier,
	WindowType_PaletteEditor,
	WindowType_CustomHud
};

class WindowContainer
{
public:
	template <class T>
	T* GetWindow(WindowType_ type) { return (T*)m_windows[type]; }
	Window* GetWindow(WindowType_ type) { return m_windows[type]; }
	void AddWindow(WindowType_ type, Window* pWindow) { m_windows[type] = pWindow; }
	virtual ~WindowContainer() {}
protected:
	std::map<WindowType_, Window*> m_windows;
};
