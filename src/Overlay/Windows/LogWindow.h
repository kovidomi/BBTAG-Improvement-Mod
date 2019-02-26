#pragma once
#include "Window.h"

class LogWindow : public Window
{
public:
	LogWindow(const std::string& windowTitle, bool windowClosable,
		ImGuiWindowFlags windowFlags = 0)
		: Window(windowTitle, windowClosable, windowFlags) {}

	~LogWindow() override = default;
	void Clear();
	void AddLog(const char* fmt, ...) IM_FMTARGS(2);
	void ToFile(FILE* file) const;

protected:
	void BeforeDraw() override;
	void Draw() override;

private:
	ImGuiTextBuffer     m_buf;
	ImGuiTextFilter     m_filter;
	ImVector<int>       m_lineOffsets;        // Index to lines offset
	float				m_prevScrollMaxY = 0;
};
