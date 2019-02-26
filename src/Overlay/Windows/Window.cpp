#include "Window.h"

void Window::Update()
{
	if (!m_windowOpen)
	{
		return;
	}

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
