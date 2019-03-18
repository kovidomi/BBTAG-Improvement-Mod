#pragma once
#include "WindowContainer.h"

class WindowContainerImpl : public WindowContainer
{
public:
	WindowContainerImpl() { InitializeWindowContainer(); }
	~WindowContainerImpl() override = default;
protected:
	void FillWindowContainer() override;
};
