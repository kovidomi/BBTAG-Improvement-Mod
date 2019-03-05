#include "Window.h"

void Window::Update()
{
	if (!m_windowOpen)
	{
		return;
	}

	BeforeDraw();

	bool* pWindowOpen = nullptr;
	if (m_windowClosable)
	{
		pWindowOpen = &m_windowOpen;
	}

	ImGui::Begin(m_windowTitle.c_str(), pWindowOpen, m_windowFlags);
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

void Window::ToggleOpen()
{
	m_windowOpen = !m_windowOpen;
}

bool Window::IsOpen() const
{
	return m_windowOpen;
}
