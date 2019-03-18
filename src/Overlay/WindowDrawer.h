#pragma once
#include "WindowContainer.h"

class WindowDrawer
{
public:
	void static DrawAllWindows(WindowContainer* pWindowContainer)
	{
		for (const auto& window : pWindowContainer->GetWindows())
		{
			window.second->Update();
		}
	}
};
