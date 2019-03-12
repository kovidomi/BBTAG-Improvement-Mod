#pragma once
#include "Overlay/Window/Window.h"
#include "HealthWindow.h"
#include "MetersWindow.h"
#include "TimerWindow.h"

#include "Core/interfaces.h"

class CustomHudWindow : public Window
{
public:
	CustomHudWindow(const std::string& windowTitle, bool windowClosable,
		ImGuiWindowFlags windowFlags)
		: Window(windowTitle, windowClosable, windowFlags)
	{
		m_healthWindowRight.SetRightSide(true);
		m_metersWindowRight.SetRightSide(true);

		m_metersWindowLeft.SetMeterData(g_interfaces.player1.GetMeters());
		m_metersWindowRight.SetMeterData(g_interfaces.player2.GetMeters());

		m_timerWindow.Open();
		m_healthWindowLeft.Open();
		m_healthWindowRight.Open();
		m_metersWindowLeft.Open();
		m_metersWindowRight.Open();
	}

	~CustomHudWindow() override = default;
	void SetScale(const ImVec2& scale);
	void SetWindowsMovable(bool isMainWindowVisible);
protected:
	void Update() override;
	void Draw() override;

private:
	bool HasNullPointerInData() const;
	void UpdateHealthWindow(bool isPlayerTwo);
	void SwapHealthBars(const CharInfo* characterActive, const CharInfo* characterInactive) const;

	TimerWindow m_timerWindow = TimerWindow("TimerWindow", false, m_windowFlags);
	HealthWindow m_healthWindowLeft = HealthWindow("PlayerOneHealthWindow", false, m_windowFlags);
	HealthWindow m_healthWindowRight = HealthWindow("PlayerTwoHealthWindow", false, m_windowFlags);
	MetersWindow m_metersWindowLeft = MetersWindow("PlayerOneMetersWindow", false, m_windowFlags);
	MetersWindow m_metersWindowRight = MetersWindow("PlayerTwoMetersWindow", false, m_windowFlags);
};
