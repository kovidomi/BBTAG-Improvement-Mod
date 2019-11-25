#pragma once
#include "Window.h"

#include "Overlay/WindowContainer/WindowContainer.h"

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
	void SetWindowContainer(WindowContainer& windowContainer);
protected:
	void BeforeDraw() override;
	void Draw() override;
private:
	void DrawDonatorsButton();
	void DrawLinkButtons() const;
	void DrawLoadedSettingsValues() const;
	void DrawCustomHudSection() const;
	void DrawCustomPalettesSection() const;
	void DrawHitboxOverlaySection() const;

	WindowContainer* m_pWindowContainer = nullptr;
};
