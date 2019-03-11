#include "CustomHudWindow.h"

void CustomHudWindow::SetScale(const ImVec2 & scale)
{
	m_healthWindowLeft.SetScale(scale);
	m_healthWindowRight.SetScale(scale);
}

void CustomHudWindow::SetWindowsMovable(bool isMainWindowVisible)
{
	if (isMainWindowVisible)
	{
		// Clear the flag
		m_windowFlags &= ~ImGuiWindowFlags_NoMove;
	}
	else
	{
		// Set the flag
		m_windowFlags |= ImGuiWindowFlags_NoMove;
	}
}

void CustomHudWindow::BeforeDraw()
{
	m_timerWindow.Update();
	m_healthWindowLeft.Update();
	m_healthWindowRight.Update();
}
