#pragma once
#include "Window.h"

class MainWindow : public Window
{
public:
	MainWindow(const std::string& windowTitle, bool windowClosable,
		ImGuiWindowFlags windowFlags = 0)
		: Window(windowTitle, windowClosable, windowFlags) {}

	~MainWindow() override = default;

protected:
	void BeforeDraw() override;
	void Draw() override;
private:
	void DrawDonatorsButton(Window& donatorsWindow);
	void DrawSingleLinkButton(const std::string& label, const wchar_t* url) const;
	void DrawLinkButtons() const;
	void DrawLoadedSettingsValues() const;
};
