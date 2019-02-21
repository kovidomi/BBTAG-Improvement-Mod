#pragma once
#include "Window.h"

#include <imgui.h>

class DonatorsWindow : Window
{
	DonatorsWindow();
	void SetWindowPosMiddleScreen() const;
	ImVec4 GetDonatorTierColor(int tierLevel) const;
public:
	void Show() override;
	~DonatorsWindow() override;
};
