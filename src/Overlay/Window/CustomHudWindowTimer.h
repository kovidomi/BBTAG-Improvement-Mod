#pragma once
#include "Window.h"

class CustomHudWindowTimer : public Window
{
public:
	CustomHudWindowTimer(const std::string& windowTitle, bool windowClosable,
		ImGuiWindowFlags windowFlags)
		: Window(windowTitle, windowClosable, windowFlags) {}

	~CustomHudWindowTimer() override = default;
protected:
	void Draw() override;
};
