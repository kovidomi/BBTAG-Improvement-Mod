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
	m_uniqueMetersWindowLeft.Open();
	m_uniqueMetersWindowRight.Open();
}

void CustomHudWindow::SetScale(const ImVec2 & scale)
{
	m_healthWindowLeft.SetScale(scale);
	m_healthWindowRight.SetScale(scale);
	m_metersWindowLeft.SetScale(scale);
	m_metersWindowRight.SetScale(scale);
	m_uniqueMetersWindowLeft.SetScale(scale);
	m_uniqueMetersWindowRight.SetScale(scale);
}

void CustomHudWindow::SetWindowsMovable(bool isMainWindowVisible)
{
	isMainWindowVisible ? m_windowFlags &= ~ImGuiWindowFlags_NoMove : m_windowFlags |= ImGuiWindowFlags_NoMove;
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

	UpdateUniqueMeterWindow(false);
	UpdateUniqueMeterWindow(true);
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
		SwapActive(&characterTop, &characterBottom);
	}

	healthWindow->SetCharObj(*characterTop);
	healthWindow->Update();
	healthWindow->SetCharObj(*characterBottom);
	healthWindow->Update();
}

void CustomHudWindow::UpdateUniqueMeterWindow(bool isPlayerTwo)
{
	const CharInfo* characterTop =
		isPlayerTwo ? g_interfaces.player2.GetChar1().GetData() : g_interfaces.player1.GetChar1().GetData();

	const CharInfo* characterBottom =
		isPlayerTwo ? g_interfaces.player2.GetChar2().GetData() : g_interfaces.player1.GetChar2().GetData();

	CharMetersWindow* uniqueMeterWindow =
		isPlayerTwo ? &m_uniqueMetersWindowRight : &m_uniqueMetersWindowLeft;

	if (characterBottom->is_char_active)
	{
		SwapActive(&characterTop, &characterBottom);
	}

	uniqueMeterWindow->SetCharObj(*characterTop);
	if (uniqueMeterWindow->CharacterHasUniqueMeter())
	{
		uniqueMeterWindow->Update();
	}

	uniqueMeterWindow->SetCharObj(*characterBottom);
	if (uniqueMeterWindow->CharacterHasUniqueMeter())
	{
		uniqueMeterWindow->Update();
	}
}

void CustomHudWindow::SwapActive(const CharInfo ** characterActive, const CharInfo ** characterInactive) const
{
	const CharInfo* temp = *characterActive;
	*characterActive = *characterInactive;
	*characterInactive = temp;
}
