#pragma once
#include "CustomHudWindowHealth.h"
#include "CustomHudWindowTimer.h"
#include "Overlay/Window/Window.h"
#include "CustomHudWindowMeters.h"

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

	CustomHudWindowTimer m_timerWindow = CustomHudWindowTimer("TimerWindow", false, 0);
	CustomHudWindowHealth m_healthWindowLeft = CustomHudWindowHealth("PlayerOneHealthWindow", false, 0);
	CustomHudWindowHealth m_healthWindowRight = CustomHudWindowHealth("PlayerTwoHealthWindow", false, 0);
	CustomHudWindowMeters m_metersWindowLeft = CustomHudWindowMeters("PlayerOneMetersWindow", false, 0);
	CustomHudWindowMeters m_metersWindowRight = CustomHudWindowMeters("PlayerTwoMetersWindow", false, 0);
};
