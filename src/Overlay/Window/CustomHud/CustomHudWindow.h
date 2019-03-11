#pragma once
#include "CustomHudWindowHealth.h"
#include "CustomHudWindowTimer.h"
#include "Overlay/Window/Window.h"

class CustomHudWindow : public Window
{
public:
	CustomHudWindow(const std::string& windowTitle, bool windowClosable,
		ImGuiWindowFlags windowFlags)
		: Window(windowTitle, windowClosable, windowFlags)
	{
		m_healthWindowLeft.SetSide(PlayerSide_OneLeft);
		m_healthWindowLeft.SetSide(PlayerSide_TwoRight);
	}

	~CustomHudWindow() override = default;
	void SetScale(const ImVec2& scale);
	void SetWindowsMovable(bool isMainWindowVisible);
protected:
	void BeforeDraw() override;
	// We do not want anything in Draw(), since this class is just a wrapper for all other custom hud windows
	void Draw() override {}

private:
	ImGuiWindowFlags m_windowFlags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar;

	CustomHudWindowTimer m_timerWindow = CustomHudWindowTimer("Timer", false, 0);
	CustomHudWindowHealth m_healthWindowLeft = CustomHudWindowHealth("PlayerOneHealthWindow", false, 0);
	CustomHudWindowHealth m_healthWindowRight = CustomHudWindowHealth("PlayerTwoHealthWindow", false, 0);
};
