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

	ImGuiWindowFlags m_windowFlags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar;

	TimerWindow m_timerWindow = TimerWindow("TimerWindow", false, 0);
	HealthWindow m_healthWindowLeft = HealthWindow("PlayerOneHealthWindow", false, 0);
	HealthWindow m_healthWindowRight = HealthWindow("PlayerTwoHealthWindow", false, 0);
	MetersWindow m_metersWindowLeft = MetersWindow("PlayerOneMetersWindow", false, 0);
	MetersWindow m_metersWindowRight = MetersWindow("PlayerTwoMetersWindow", false, 0);
};
