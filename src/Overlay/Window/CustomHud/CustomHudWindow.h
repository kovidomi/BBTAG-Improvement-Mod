#pragma once
#include "Overlay/Window/Window.h"
#include "HealthWindow.h"
#include "MetersWindow.h"
#include "TimerWindow.h"

class CustomHudWindow : public Window
{
public:
	CustomHudWindow(const std::string& windowTitle, bool windowClosable,
		ImGuiWindowFlags windowFlags)
		: Window(windowTitle, windowClosable, windowFlags)
	{
		m_healthWindowRight.SetRightSide(true);
		m_metersWindowRight.SetRightSide(true);
	}

	~CustomHudWindow() override = default;
	void SetScale(const ImVec2& scale);
	void SetWindowsMovable(bool isMainWindowVisible);
protected:
	void BeforeDraw() override;
	// We do not want anything in Draw(), since this class is just a wrapper for all other custom hud windows
	void Draw() override {}

private:
	void UpdateHealthWindow(bool isPlayerTwo);
	void SwapHealthBars(const CharInfo* characterActive, const CharInfo* characterInactive) const;

	TimerWindow m_timerWindow = TimerWindow("TimerWindow", false, m_windowFlags);
	HealthWindow m_healthWindowLeft = HealthWindow("PlayerOneHealthWindow", false, m_windowFlags);
	HealthWindow m_healthWindowRight = HealthWindow("PlayerTwoHealthWindow", false, m_windowFlags);
	MetersWindow m_metersWindowLeft = MetersWindow("PlayerOneMetersWindow", false, m_windowFlags);
	MetersWindow m_metersWindowRight = MetersWindow("PlayerTwoMetersWindow", false, m_windowFlags);
};
