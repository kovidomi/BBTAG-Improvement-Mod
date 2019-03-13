#include "CustomHudWindow.h"

#include "Core/interfaces.h"
#include "Core/logger.h"

CustomHudWindow::CustomHudWindow(const std::string & windowTitle, bool windowClosable, ImGuiWindowFlags windowFlags)
	:Window(windowTitle, windowClosable, windowFlags)
{
	m_healthWindowRight.SetRightSide(true);
	m_metersWindowRight.SetRightSide(true);

	// TODO: fix this doublepointer abomination
	m_metersWindowLeft.SetMeterData(g_interfaces.player1.GetMetersMemberRef());
	m_metersWindowRight.SetMeterData(g_interfaces.player2.GetMetersMemberRef());

	m_timerWindow.Open();
	m_healthWindowLeft.Open();
	m_healthWindowRight.Open();
	m_metersWindowLeft.Open();
	m_metersWindowRight.Open();
}

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

void CustomHudWindow::Update()
{
	if (HasNullPointerInData())
	{
		return;
	}

	if (m_windowOpen || Settings::settingsIni.forcecustomhud)
	{
		Draw();
	}
}

void CustomHudWindow::Draw()
{
	m_timerWindow.Update();

	UpdateHealthWindow(false);
	UpdateHealthWindow(true);

	m_metersWindowLeft.Update();
	m_metersWindowRight.Update();
}

bool CustomHudWindow::HasNullPointerInData() const
{
	if (g_interfaces.player1.GetChar1().IsCharDataNullPtr()
		|| g_interfaces.player1.GetChar2().IsCharDataNullPtr()
		|| g_interfaces.player2.GetChar1().IsCharDataNullPtr()
		|| g_interfaces.player2.GetChar2().IsCharDataNullPtr())
	{
		LOG(2, "One of Player.Char() is NULL !!!!!\n");
		return true;
	}

	if (m_metersWindowLeft.IsMetersDataNullptr() || m_metersWindowRight.IsMetersDataNullptr())
	{
		LOG(2, "One of Player.Meter() is NULL !!!!\n");
		return true;
	}

	return false;
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
