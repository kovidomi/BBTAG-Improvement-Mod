#pragma once
#include "Overlay/Window/Window.h"
#include "HealthWindow.h"
#include "MetersWindow.h"
#include "TimerWindow.h"
#include "UniqueMetersWindow.h"

#include "Core/interfaces.h"

class CustomHudWindow : public Window
{
public:
	CustomHudWindow(const std::string& windowTitle, bool windowClosable, ImGuiWindowFlags windowFlags);
	~CustomHudWindow() override = default;
	void SetScale(const ImVec2& scale);
	void DrawResetWindowsPositionsButton();
	void DrawShowCustomHudWindowCheckbox();
	void DrawSetWindowsMovableCheckbox();
protected:
	void Update() override;
	void BeforeDraw() override;
	void AfterDraw() override;
	void Draw() override;
private:
	void SetWindowsMovable(bool movable);
	bool HasNullPointerInData() const;
	void PushStyles();
	void PopStyles();
	void UpdateHealthWindow(bool isPlayerTwo);
	void UpdateUniqueMeterWindow(bool isPlayerTwo);
	void SwapActive(const CharInfo** characterActive, const CharInfo** characterInactive) const;

	bool m_isBlazeActivate = false;

	TimerWindow m_timerWindow = TimerWindow("TimerWindow", false, m_windowFlags);
	HealthWindow m_healthWindowLeft = HealthWindow("PlayerOneHealthWindow", false, m_windowFlags);
	HealthWindow m_healthWindowRight = HealthWindow("PlayerTwoHealthWindow", false, m_windowFlags);
	MetersWindow m_metersWindowLeft = MetersWindow("PlayerOneMetersWindow", false, m_windowFlags);
	MetersWindow m_metersWindowRight = MetersWindow("PlayerTwoMetersWindow", false, m_windowFlags);
	UniqueMetersWindow m_uniqueMetersWindowLeft = UniqueMetersWindow("PlayerOneUniqueMetersWindow", false, m_windowFlags);
	UniqueMetersWindow m_uniqueMetersWindowRight = UniqueMetersWindow("PlayerTwoUniqueMetersWindow", false, m_windowFlags);
};
