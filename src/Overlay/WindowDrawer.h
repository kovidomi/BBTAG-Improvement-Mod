#pragma once
#include "WindowContainer.h"

namespace WindowDrawer
{
	inline void DrawAllWindows(WindowContainer* pWindowContainer)
	{
		for (const auto& window : pWindowContainer->GetWindows())
		{
			window.second->Update();
		}
	}
};
