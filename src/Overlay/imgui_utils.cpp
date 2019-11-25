#include "imgui_utils.h"

#include <windows.h>

void HoverTooltip(const char* text)
{
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::TextUnformatted(text);
		ImGui::EndTooltip();
	}
}

void DrawUrlButton(const std::string& label, const wchar_t* url, const ImVec2& btnSize)
{
	if (ImGui::Button(label.c_str(), btnSize))
	{
		ShellExecute(nullptr, L"open", url, nullptr, nullptr, SW_SHOWNORMAL);
	}
}