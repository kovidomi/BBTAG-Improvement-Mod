#include "TimerWindow.h"

#include "Core/interfaces.h"

void TimerWindow::Draw()
{
	if(!g_gameVals.pGameTimer)
	{
		return;
	}

	ImGui::TextUnformatted("TIME");

	char time[8];
	sprintf(time, "%d", *g_gameVals.pGameTimer);
	// Position the curor so the time string will be drawn at the middle of window
	ImGui::SetCursorPosX((ImGui::GetWindowSize().x / 2) - (ImGui::CalcTextSize(time).x / 2));
	ImGui::TextUnformatted(time);
}
