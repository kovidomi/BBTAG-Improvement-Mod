#pragma once
#include "Window.h"

class UpdateNotifierWindow : public Window
{
public:
	UpdateNotifierWindow(const std::string& windowTitle, bool windowClosable,
		ImGuiWindowFlags windowFlags = 0);
	~UpdateNotifierWindow() override = default;

protected:
	void BeforeDraw() override;
	void Draw() override;
};
