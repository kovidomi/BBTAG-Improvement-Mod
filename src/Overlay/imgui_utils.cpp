#include "imgui_utils.h"

#include <imgui.h>

void HoverTooltip(const char* text)
{
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::TextUnformatted(text);
		ImGui::EndTooltip();
	}
}
