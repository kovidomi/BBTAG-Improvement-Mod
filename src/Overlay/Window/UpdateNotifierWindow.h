#pragma once
#include "Window.h"

class UpdateNotifierWindow : public Window
{
public:
	UpdateNotifierWindow(const std::string& windowTitle, bool windowClosable,
		ImGuiWindowFlags windowFlags = 0)
		: Window(windowTitle, windowClosable, windowFlags) {}

	~UpdateNotifierWindow() override = default;

protected:
	void BeforeDraw() override;
	void Draw() override;
};
