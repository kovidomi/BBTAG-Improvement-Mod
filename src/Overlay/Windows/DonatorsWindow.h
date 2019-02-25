#pragma once
#include "Window.h"

#include <imgui.h>

class DonatorsWindow : Window
{
public:
	DonatorsWindow() = default;
	void BeforeDraw() override;
	void Draw() override;
	void AfterDraw() override;
	~DonatorsWindow() override = default;
private:
	char CalculateAnimatedTitleChar() const;
	char* ConstructWindowTitle(char* outBuffer) const;
	ImVec4 GetDonatorTierColor(int tierLevel) const;
	void PrintDonators() const;
	void DrawOkButton() const;
	void PositionWindowToMiddleScreen() const;
};
