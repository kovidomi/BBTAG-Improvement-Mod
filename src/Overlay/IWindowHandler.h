#pragma once
#include "Window/Window.h"

enum WindowType_;

class IWindowHandler
{
public:
	virtual Window* GetWindow(WindowType_ type) = 0;
	virtual ~IWindowHandler() {}
};
