#pragma once
#include "Window.h"

#include <imgui.h>

class DonatorsWindow : public Window
{
public:
	DonatorsWindow(const std::string& title, bool closable,
		ImGuiWindowFlags windowFlags)
		: Window(title, closable, windowFlags) {}

	void BeforeDraw() override;
	void Draw() override;
	void AfterDraw() override;
	~DonatorsWindow() override = default;
private:
	char CalculateAnimatedTitleChar() const;
	char* ConstructWindowTitle(char* outBuffer) const;
	ImVec4 GetDonatorTierColor(int tierLevel) const;
	void PrintDonators() const;
	bool DrawOkButton() const;
	void PositionWindowToMiddleScreen() const;
};
