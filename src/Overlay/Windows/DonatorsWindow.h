#pragma once
#include "Window.h"

#include <imgui.h>

class DonatorsWindow : Window
{
	DonatorsWindow();
	char CalculateAnimatedTitleChar() const;
	const char* ConstructWindowTitle(char* outBuffer) const;
	ImVec4 GetDonatorTierColor(int tierLevel) const;
	void PrintDonators() const;
	void DrawOkButton() const;
	void PositionWindowToMiddleScreen() const;
public:
	void Show() override;
	~DonatorsWindow() override;
};