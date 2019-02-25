#include "Window.h"

Window::Window(std::string title, bool closable, ImGuiWindowFlags windowFlags)
{
	m_windowClosable = closable;
	m_windowFlags = windowFlags;
	m_windowTitle = title;
}

void Window::Update()
{
	BeforeDraw();

	bool* p_open = nullptr;
	if (m_windowClosable)
	{
		p_open = &m_windowOpen;
	}

	ImGui::Begin(m_windowTitle.c_str(), p_open, m_windowFlags);
	Draw();
	ImGui::End();

	AfterDraw();
}

void Window::Open()
{
	m_windowOpen = true;
}

void Window::Close()
{
	m_windowOpen = false;
}
