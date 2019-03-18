#pragma once
#include "WindowDrawer.h"
#include "WindowContainer.h"

enum WindowType_
{
	WindowType_Main,
	WindowType_Debug,
	WindowType_Log,
	WindowType_Donators,
	WindowType_UpdateNotifier,
	WindowType_PaletteEditor,
	WindowType_CustomHud
};

class WindowManager : public WindowContainer
{
public:
	~WindowManager() override = default;
protected:
	void FillWindowContainer() override;
};
