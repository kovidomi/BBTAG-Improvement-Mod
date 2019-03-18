#include "CustomHudWindow.h"

#include "Core/interfaces.h"
#include "Core/logger.h"

CustomHudWindow::CustomHudWindow(const std::string & windowTitle, bool windowClosable, ImGuiWindowFlags windowFlags)
	:Window(windowTitle, windowClosable, windowFlags)
{
	m_healthWindowRight.SetRightSide(true);
	m_metersWindowRight.SetRightSide(true);
	m_uniqueMetersWindowRight.SetRightSide(true);

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

void CustomHudWindow::DrawResetWindowsPositionsButton()
{
	const ImVec2 middleScreen = ImVec2(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2);
	if (ImGui::Button("Reset custom HUD positions"))
	{
		ImGui::SetWindowPos("TimerWindow", middleScreen);
		ImGui::SetWindowPos("PlayerOneHealthWindow", middleScreen);
		ImGui::SetWindowPos("PlayerTwoHealthWindow", middleScreen);
		ImGui::SetWindowPos("PlayerOneMetersWindow", middleScreen);
		ImGui::SetWindowPos("PlayerTwoMetersWindow", middleScreen);
		ImGui::SetWindowPos("PlayerOneUniqueMetersWindow", middleScreen);
		ImGui::SetWindowPos("PlayerTwoUniqueMetersWindow", middleScreen);
	}
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::Text("Used to recover elements of the custom HUD that have\nbecome unrecoverable due to going beyond the screen");
		ImGui::EndTooltip();
	}
}

void CustomHudWindow::DrawShowCustomHudWindowCheckbox()
{
	ImGui::Checkbox("Show Custom HUD", &m_windowOpen);
}

void CustomHudWindow::DrawSetWindowsMovableCheckbox()
{
	static bool moveWindows = false;
	if(ImGui::Checkbox("Move windows", &moveWindows))
	{
		SetWindowsMovable(moveWindows);
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
		BeforeDraw();
		Draw();
		AfterDraw();
	}
}

void CustomHudWindow::BeforeDraw()
{
	if (Settings::settingsIni.forcecustomhud && *g_gameVals.pIsHUDHidden == 0)
	{
		*g_gameVals.pIsHUDHidden = 1;
	}

	PushStyles();
}

void CustomHudWindow::AfterDraw()
{
	PopStyles();
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

void CustomHudWindow::PushStyles()
{
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4.0f, 4.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(1.0f, 1.0f));

	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f)); //black
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.286f, 0.286f, 0.286f, 0.54f)); //grey

	m_isBlazeActivate =
		g_interfaces.player1.GetMeters()->is_blaze_active || g_interfaces.player2.GetMeters()->is_blaze_active;

	if (m_isBlazeActivate)
	{
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 1.0f, 1.0f, 1.0f)); //cyan
		ImGui::PushStyleColor(ImGuiCol_BorderShadow, ImVec4(0.0f, 1.0f, 1.0f, 1.0f)); //cyan
	}

	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
}

void CustomHudWindow::PopStyles()
{
	ImGui::PopFont();

	if (m_isBlazeActivate)
	{
		ImGui::PopStyleColor(2);
	}

	ImGui::PopStyleColor(2);
	ImGui::PopStyleVar(3);
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

	UniqueMetersWindow* uniqueMeterWindow =
		isPlayerTwo ? &m_uniqueMetersWindowRight : &m_uniqueMetersWindowLeft;

	if (characterBottom->is_char_active)
	{
		SwapActive(&characterTop, &characterBottom);
	}

	uniqueMeterWindow->SetCharObj(*characterTop);
	if (uniqueMeterWindow->CharacterHasUniqueMeterToDraw())
	{
		uniqueMeterWindow->Update();
	}

	uniqueMeterWindow->SetCharObj(*characterBottom);
	if (uniqueMeterWindow->CharacterHasUniqueMeterToDraw())
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
