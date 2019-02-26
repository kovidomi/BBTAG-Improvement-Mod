#pragma once
#include <imgui.h>

#include <string>

class Window
{
	// virtual void PushStyles() = 0;
	// virtual void PopStyles() = 0;
	// void SavePreviousGlobalStyles();
	// void RestorePreviousGlobalStyles();
public:
	Window(const std::string& windowTitle, bool windowClosable, ImGuiWindowFlags windowFlags)
		: m_windowTitle(windowTitle),
		m_windowClosable(windowClosable),
		m_windowFlags(windowFlags) {}

	virtual ~Window() = default;
	void Update();
	void Open();
	void Close();

protected:
	virtual void BeforeDraw() {}
	virtual void Draw() = 0;
	virtual void AfterDraw() {}

	std::string m_windowTitle;
	bool m_windowClosable;
	ImGuiWindowFlags m_windowFlags;
	bool m_windowOpen = false;
};
