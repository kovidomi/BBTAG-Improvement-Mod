#include "Window.h"

void Window::Update()
{
	if (!m_windowOpen)
	{
		return;
	}

	BeforeDraw();

	ImGui::Begin(m_windowTitle.c_str(), GetWindowOpenPointer(), m_windowFlags);
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

bool * Window::GetWindowOpenPointer()
{
	if (m_windowClosable)
	{
		return &m_windowOpen;
	}

	return nullptr;
}
