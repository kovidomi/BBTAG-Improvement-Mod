#include "DonatorsWindow.h"

#include "Core/info.h"
#include "Overlay/imgui_utils.h"
#include "Web/donators_fetch.h"

#include <cstdio>

const ImVec4 COLOR_PLATINUM    (0.328f, 1.000f, 0.901f, 1.000f);
const ImVec4 COLOR_GOLD        (1.000f, 0.794f, 0.000f, 1.000f);
const ImVec4 COLOR_SILVER      (0.848f, 0.848f, 0.848f, 1.000f);
const ImVec4 COLOR_BRONZE      (0.824f, 0.497f, 0.170f, 1.000f);
const ImVec4 COLOR_DEFAULT     (1.000f, 1.000f, 1.000f, 1.000f);

const std::vector<ImVec4> DONATOR_TIER_COLORS = { COLOR_PLATINUM, COLOR_GOLD, COLOR_SILVER, COLOR_BRONZE };
ImVec2 backupWindowTitleAlign;

void DonatorsWindow::BeforeDraw()
{
	char titleBuffer[128];
	m_windowTitle = ConstructWindowTitle(titleBuffer);

	backupWindowTitleAlign = ImGui::GetStyle().WindowTitleAlign;
	ImGui::GetStyle().WindowTitleAlign = ImVec2(0.5f, 0.5f); // middle
	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 1.0f);
}

void DonatorsWindow::Draw()
{
	ImGui::BeginChild("donator_list", ImVec2(275, 400), true);
	PrintDonators();
	ImGui::EndChild();

	DrawDonateButton();
	ImGui::TextUnformatted(" ");
	const bool closePressed = DrawCloseButton();
	if (closePressed)
	{
		Close();
	}
	PositionWindowToMiddleScreen();
}

void DonatorsWindow::AfterDraw()
{
	ImGui::PopStyleVar();
	ImGui::GetStyle().WindowTitleAlign = backupWindowTitleAlign;
}

char DonatorsWindow::CalculateAnimatedTitleChar() const
{
	const char animationFrames[] = "|/-\\";
	const float animationSpeed = 0.25f;
	const int currentAnimationFrameIndex = (int)(ImGui::GetTime() / animationSpeed) & 3;

	return animationFrames[currentAnimationFrameIndex];
}

char* DonatorsWindow::ConstructWindowTitle(char* outBuffer) const
{
	const char animatedCharacter = CalculateAnimatedTitleChar();
	sprintf(outBuffer, "%c SUPPORTERS %c###Donators", animatedCharacter, animatedCharacter);

	return outBuffer;
}

void DonatorsWindow::PositionWindowToMiddleScreen() const
{
	// Set window to the middle of the screen, on the first call the windowsize is always minimum
	const float minWindowHeight = 51.0f;
	const ImVec2 windowSize = ImGui::GetWindowSize();

	if (minWindowHeight < windowSize.y)
	{
		const ImGuiIO io = ImGui::GetIO();
		ImGui::SetWindowPos(
			ImVec2((io.DisplaySize.x * 0.5f) - (windowSize.x / 2), (io.DisplaySize.y * 0.5f) - (windowSize.y / 2)),
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

void DonatorsWindow::PrintDonators() const
{
	const auto donatorNames = GetDonatorNames();
	const auto donatorTiers = GetDonatorTiers();

	int previousTierLevel = 100;

	for (int i = 0; i < donatorNames.size(); i++)
	{
		int tierLevel = 99;
		if (i < donatorTiers.size())
		{
			tierLevel = donatorTiers[i];
		}

		if (previousTierLevel != tierLevel)
		{
			if (tierLevel == 0)
			{
				TextColoredAlignedHorizontalCenter(COLOR_PLATINUM, "----  TOP DONATOR  ----");
			}
			else
			{
				ImGui::TextUnformatted(" ");
				switch (tierLevel)
				{
				case 1:
					TextColoredAlignedHorizontalCenter(COLOR_GOLD, "----  GOLD DONATORS  ----");
					break;
				case 2:
					TextColoredAlignedHorizontalCenter(COLOR_SILVER, "----  SILVER DONATORS  ----");
					break;
				case 3:
					TextColoredAlignedHorizontalCenter(COLOR_BRONZE, "----  BRONZE DONATORS  ----");
					break;
				}
			}
		}

		const char* donatorName = donatorNames[i].c_str();
		TextColoredAlignedHorizontalCenter(GetDonatorTierColor(tierLevel), donatorName);
		previousTierLevel = tierLevel;
	}
}

void DonatorsWindow::DrawDonateButton() const
{
	const ImVec2 btnSize(100, 30);
	AlignItemHorizontalCenter(btnSize.x);

	ButtonUrl("DONATE", MOD_LINK_DONATE, btnSize);
}

bool DonatorsWindow::DrawCloseButton() const
{
	const ImVec2 btnSize(100, 30);
	AlignItemHorizontalCenter(btnSize.x);

	if (ImGui::Button("CLOSE", btnSize))
	{
		return true;
	}

	return false;
}
