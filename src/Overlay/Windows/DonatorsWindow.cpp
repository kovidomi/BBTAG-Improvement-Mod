#include "DonatorsWindow.h"

#include "Web/donators_fetch.h"

#include <cstdio>

const ImVec4 COLOR_PLATINUM    (0.328f, 1.000f, 0.901f, 1.000f);
const ImVec4 COLOR_GOLD        (1.000f, 0.794f, 0.000f, 1.000f);
const ImVec4 COLOR_SILVER      (0.848f, 0.848f, 0.848f, 1.000f);
const ImVec4 COLOR_BRONZE      (0.824f, 0.497f, 0.170f, 1.000f);
const ImVec4 COLOR_DEFAULT     (1.000f, 1.000f, 1.000f, 1.000f);
const ImVec4 COLOR_TRANSPARENT (0.000f, 0.000f, 0.000f, 0.000f);

const std::vector<ImVec4> DONATOR_TIER_COLORS = { COLOR_PLATINUM, COLOR_GOLD, COLOR_SILVER, COLOR_BRONZE };

DonatorsWindow::DonatorsWindow()
{
}

void DonatorsWindow::SetWindowPosMiddleScreen() const
{
	// Set window to the middle of the screen, on the first call the windowsize is always minimum
	const float minWindowHeight = 51.0f;
	const ImVec2 wndSize = ImGui::GetWindowSize();

	if (minWindowHeight < wndSize.y)
	{
		const ImGuiIO io = ImGui::GetIO();
		ImGui::SetWindowPos(
			ImVec2((io.DisplaySize.x * 0.5f) - (wndSize.x / 2), (io.DisplaySize.y * 0.5f) - (wndSize.y / 2)),
			ImGuiCond_Once
		);
	}
}

ImVec4 DonatorsWindow::GetDonatorTierColor(int tierLevel) const
{
	if (DONATOR_TIER_COLORS.size() - 1 < tierLevel)
	{
		return COLOR_DEFAULT;
	}
	
	return DONATOR_TIER_COLORS[tierLevel];
}

void DonatorsWindow::Show()
{
	const ImVec2 origWindowTitleAlign = ImGui::GetStyle().WindowTitleAlign;

	ImGui::GetStyle().WindowTitleAlign = ImVec2(0.5f, 0.5f); //middle
	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 1.0f);
	ImGui::SetNextWindowSizeConstraints(ImVec2(200, 50), ImVec2(500, 500));
	// ImGui::SetNextWindowPosCenter(ImGuiCond_FirstUseEver);
	const ImVec2 okBtnSize(100, 30);

	char buf[128];
	const char flipper = "|/-\\"[(int)(ImGui::GetTime() / 0.25f) & 3];
	sprintf(buf, "%c SUPPORTERS %c###Donators", flipper, flipper);

	ImGui::Begin(buf, NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);

	const auto donatorNames = GetDonatorNames();
	const auto donatorTiers = GetDonatorTiers();
	ImVec4 donatorColor = COLOR_BRONZE;

	for (int i = 0; i < donatorNames.size(); i++)
	{
		if (i == 0)
		{
			sprintf(buf, "%s", "TOP DONATOR");

			ImGui::TextColored(COLOR_TRANSPARENT, buf);
			float width = ImGui::GetItemRectSize().x;
			ImGui::SetCursorPosX(ImGui::GetWindowSize().x / 2 - (width / 2));

			ImGui::TextColored(COLOR_PLATINUM, buf);

			float height = ImGui::GetItemRectSize().y;
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() - height - (ImGui::GetStyle().ItemSpacing.y * 2));
		}

		sprintf(buf, "%s", donatorNames[i].c_str());

		ImGui::TextColored(COLOR_TRANSPARENT, buf);
		float width = ImGui::GetItemRectSize().x;
		ImGui::SetCursorPosX(ImGui::GetWindowSize().x / 2 - (width / 2));

		int tierLevel = 99;
		if (i < donatorTiers.size())
		{
			tierLevel = donatorTiers[i];
		}

		donatorColor = GetDonatorTierColor(tierLevel);
		ImGui::TextColored(donatorColor, buf);
	}
	ImGui::Text("");

	ImGui::SetCursorPosX(ImGui::GetWindowSize().x / 2 - (okBtnSize.x / 2));

	if (ImGui::Button("OK", okBtnSize))
	{
		show_donators_window = false;
	}

	SetWindowPosMiddleScreen();

	ImGui::End();

	ImGui::PopStyleVar();
	ImGui::GetStyle().WindowTitleAlign = origWindowTitleAlign;
}

DonatorsWindow::~DonatorsWindow()
{
}
