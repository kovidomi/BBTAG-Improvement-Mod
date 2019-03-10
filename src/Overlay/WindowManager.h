#pragma once
#include "CustomHud.h"
#include "WindowContainer.h"

class WindowManager : public WindowContainer
{
public:
	WindowManager();
	~WindowManager() override;
	void DrawAllWindows();
private:
	void InitWindows();
	CustomHud m_customHud = CustomHud();

	bool m_showCustomHud = false;
	//ImVec2 m_middleScreen;
};
