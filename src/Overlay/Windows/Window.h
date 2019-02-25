#pragma once

class Window
{
	// bool m_windowOpen;

	// virtual void PushStyles() = 0;
	// virtual void PopStyles() = 0;
	// void SavePreviousGlobalStyles();
	// void RestorePreviousGlobalStyles();
public:
	virtual void Show() = 0;
	virtual ~Window() = default;
};
