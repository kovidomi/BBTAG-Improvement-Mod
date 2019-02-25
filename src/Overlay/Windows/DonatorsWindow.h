#pragma once
#include "Window.h"

#include <imgui.h>

class DonatorsWindow : Window
{
	DonatorsWindow() = default;
	char CalculateAnimatedTitleChar() const;
	void ConstructWindowTitle(char* outBuffer) const;
	ImVec4 GetDonatorTierColor(int tierLevel) const;
	void PrintDonators() const;
	void DrawOkButton() const;
	void PositionWindowToMiddleScreen() const;
public:
	void Show() override;
	~DonatorsWindow() override = default;
};
