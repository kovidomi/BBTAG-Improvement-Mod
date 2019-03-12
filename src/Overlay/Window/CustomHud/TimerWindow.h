#pragma once
#include "Overlay/Window/Window.h"

class TimerWindow : public Window
{
public:
	TimerWindow(const std::string& windowTitle, bool windowClosable,
		ImGuiWindowFlags windowFlags)
		: Window(windowTitle, windowClosable, windowFlags) {}

	~TimerWindow() override = default;
protected:
	void Draw() override;
};
