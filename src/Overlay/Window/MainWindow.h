#pragma once
#include "Window.h"
#include "DonatorsWindow.h"

#include "Overlay/WindowContainer.h"

class MainWindow : public Window
{
public:
	MainWindow(const std::string& windowTitle, bool windowClosable,
		ImGuiWindowFlags windowFlags = 0)
		: Window(windowTitle, windowClosable, windowFlags)
	{
		SetMainWindowTitle();
	}

	~MainWindow() override = default;
	void SetMainWindowTitle(const std::string text = "");
	void SetWindowHandler(WindowContainer& windowContainer);
protected:
	void BeforeDraw() override;
	void Draw() override;
private:
	void DrawDonatorsButton();
	void DrawSingleLinkButton(const std::string& label, const wchar_t* url) const;
	void DrawLinkButtons() const;
	void DrawLoadedSettingsValues() const;

	WindowContainer* m_pWindowContainer = nullptr;
};
