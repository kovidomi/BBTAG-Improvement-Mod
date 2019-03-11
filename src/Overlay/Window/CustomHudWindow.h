#pragma once
#include "Window.h"
#include "CustomHudWindowHealth.h"
#include "CustomHudWindowTimer.h"

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
protected:
	void BeforeDraw() override;
	// We do not want anything in Draw(), since this class is just a wrapper for all other custom hud windows
	void Draw() override {}

	CustomHudWindowTimer m_timer = CustomHudWindowTimer("Timer", false, 0);
	CustomHudWindowHealth m_healthWindowLeft = CustomHudWindowHealth("PlayerOneHealthWindow", false, 0);
	CustomHudWindowHealth m_healthWindowRight = CustomHudWindowHealth("PlayerTwoHealthWindow", false, 0);
};
