#pragma once
#include "Window.h"

class DebugWindow : public Window
{
public:
	DebugWindow(const std::string& windowTitle, bool windowClosable,
		ImGuiWindowFlags windowFlags = 0)
		: Window(windowTitle, windowClosable, windowFlags) {}

	~DebugWindow() override = default;

protected:
	void Draw() override;
};
