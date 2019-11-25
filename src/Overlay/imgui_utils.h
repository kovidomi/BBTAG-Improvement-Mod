#pragma once

#include "imgui.h"

#include <string>

void HoverTooltip(const char* text);
void DrawUrlButton(const std::string& label, const wchar_t* url, const ImVec2& btnSize = ImVec2(0, 0));