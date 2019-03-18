#pragma once
#include "Window/Window.h"
#include <map>

enum WindowType_;

typedef std::map<WindowType_, Window*> WindowMap;

class WindowContainer
{
public:
	WindowContainer() { InitWindowContainer(); }
	virtual ~WindowContainer()
	{
		for (const auto& window : m_windows)
		{
			delete window.second;
		}
	}
	template <class T>
	T* GetWindow(WindowType_ type) { return (T*)m_windows[type]; }
	Window* GetWindow(WindowType_ type) { return m_windows[type]; }
	const WindowMap& GetWindows() const { return m_windows; }
protected:
	// Call AddWindow for each window in here 
	virtual void FillWindowContainer() = 0;
	void AddWindow(WindowType_ type, Window* pWindow) { m_windows[type] = pWindow; }
private:
	void InitWindowContainer() { FillWindowContainer(); }

	WindowMap m_windows;
};
