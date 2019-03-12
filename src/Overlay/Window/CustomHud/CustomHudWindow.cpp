#include "CustomHudWindow.h"

#include "Core/interfaces.h"

void CustomHudWindow::SetScale(const ImVec2 & scale)
{
	m_healthWindowLeft.SetScale(scale);
	m_healthWindowRight.SetScale(scale);
	m_metersWindowLeft.SetScale(scale);
	m_metersWindowRight.SetScale(scale);
}

void CustomHudWindow::SetWindowsMovable(bool isMainWindowVisible)
{
	if (isMainWindowVisible)
	{
		// Clear flag
		m_windowFlags &= ~ImGuiWindowFlags_NoMove;
	}
	else
	{
		// Set flag
		m_windowFlags |= ImGuiWindowFlags_NoMove;
	}
}

void CustomHudWindow::BeforeDraw()
{
	m_timerWindow.Update();

	UpdateHealthWindow(false);
	UpdateHealthWindow(true);

	m_metersWindowLeft.Update();
	m_metersWindowRight.Update();
}

void CustomHudWindow::UpdateHealthWindow(bool isPlayerTwo)
{
	const CharInfo* characterTop =
		isPlayerTwo ? g_interfaces.player2.GetChar1().GetData() : g_interfaces.player1.GetChar1().GetData();

	const CharInfo* characterBottom =
		isPlayerTwo ? g_interfaces.player2.GetChar2().GetData() : g_interfaces.player1.GetChar2().GetData();

	HealthWindow* healthWindow =
		isPlayerTwo ? &m_healthWindowRight : &m_healthWindowLeft;

	if (characterBottom->is_char_active)
	{
		SwapHealthBars(characterTop, characterBottom);
	}

	healthWindow->SetCharObj(*characterTop);
	healthWindow->Update();
	healthWindow->SetCharObj(*characterBottom);
	healthWindow->Update();
}

void CustomHudWindow::SwapHealthBars(const CharInfo * characterActive, const CharInfo * characterInactive) const
{
	const CharInfo* temp = characterActive;
	characterActive = characterInactive;
	characterInactive = temp;
}
