#pragma once
#include "WindowContainer.h"

class WindowDrawer
{
public:
	virtual void DrawAllWindows(WindowContainer* pWindowContainer)
	{
		for (const auto& window : pWindowContainer->GetWindows())
		{
			window.second->Update();
		}
	}

	virtual ~WindowDrawer() = default;
};
