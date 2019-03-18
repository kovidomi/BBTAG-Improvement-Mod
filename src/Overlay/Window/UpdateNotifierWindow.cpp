#include "UpdateNotifierWindow.h"

#include "Web/update_check.h"

void UpdateNotifierWindow::BeforeDraw()
{
	ImGui::SetNextWindowPosCenter(ImGuiCond_Once);
	ImGui::SetNextWindowSizeConstraints(ImVec2(200, 50), ImVec2(500, 500));
}

void UpdateNotifierWindow::Draw()
{
	ImGui::Text("BBTAG Improvement Mod %s has been released!", GetNewVersionNum().c_str());

	const ImVec2 buttonSize = ImVec2(100, 30);
	ImGui::SetCursorPosX(ImGui::GetWindowSize().x / 2 - (buttonSize.x / 2));

	if (ImGui::Button("OK", buttonSize))
	{
		Close();
	}
}
