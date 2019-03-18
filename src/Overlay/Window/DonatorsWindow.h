#pragma once
#include "Window.h"

#include <imgui.h>

class DonatorsWindow : public Window
{
public:

	DonatorsWindow(const std::string& windowTitle, bool windowClosable,
		ImGuiWindowFlags windowFlags)
		: Window(windowTitle, windowClosable, windowFlags) {}
	~DonatorsWindow() override = default;

protected:
	void BeforeDraw() override;
	void Draw() override;
	void AfterDraw() override;

private:
	char CalculateAnimatedTitleChar() const;
	char* ConstructWindowTitle(char* outBuffer) const;
	ImVec4 GetDonatorTierColor(int tierLevel) const;
	void PrintDonators() const;
	bool DrawOkButton() const;
	void PositionWindowToMiddleScreen() const;
};
