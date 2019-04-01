#pragma once
#include "Window.h"

// TODO: Show notification text on main window's title without popup

class NotificationWindow : public Window
{
public:
	NotificationWindow(const std::string& windowTitle, bool windowClosable,
		ImGuiWindowFlags windowFlags = 0)
		: Window(windowTitle, windowClosable, windowFlags) {}
	~NotificationWindow() override = default;
	void SetNotification(const std::string& text, float timeToShowInSec = 5.0f);
protected:
	void BeforeDraw() override;
	void Draw() override;
	void AfterDraw() override;
private:
	void SetNotificationTextOnMainWindowTitle();
	void UpdateNotificationTimer();

	std::string m_notificationText;
	float m_notificationTimer = 0.0f;
};
