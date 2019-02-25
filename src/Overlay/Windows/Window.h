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
	virtual void BeforeDraw() = 0;
	virtual void Draw() = 0;
	virtual void AfterDraw() = 0;
	virtual ~Window() = default;
	Window(std::string title, bool closable, ImGuiWindowFlags windowFlags);
	void Update();
	void Open();
	void Close();

protected:
	ImGuiWindowFlags m_windowFlags;
	bool m_windowOpen = false;
	bool m_windowClosable;
	std::string m_windowTitle;
};
