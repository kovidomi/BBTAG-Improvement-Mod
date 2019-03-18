#pragma once
#include "WindowContainer/WindowContainer.h"

class WindowDrawer
{
public:
	static void DrawAllWindows(WindowContainer* pWindowContainer)
	{
		for (const auto& window : pWindowContainer->GetWindows())
		{
			window.second->Update();
		}
	}
};
