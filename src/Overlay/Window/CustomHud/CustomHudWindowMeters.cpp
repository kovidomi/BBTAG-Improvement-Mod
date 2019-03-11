#include "CustomHudWindowMeters.h"

const ImVec4 COLOR_CROSS_BAR_DEFAULT      (0.150f, 1.000f, 1.000f, 1.000f);
const ImVec4 COLOR_SKILL_BAR_DEFAULT      (1.000f, 0.340f, 0.000f, 1.000f);
const ImVec4 COLOR_SKILL_BAR_BLAZE_ACTIVE (0.450f, 1.000f, 1.000f, 1.000f);
const ImVec4 COLOR_CROSS_BAR_BLAZE_ACTIVE (0.000f, 0.400f, 1.000f, 1.000f);
const ImVec4 COLOR_BLAZE_VALUE            (1.000f, 0.400f, 1.000f, 1.000f);
const ImVec4 COLOR_ASTRAL_AVAILABLE       (1.000f, 0.949f, 0.000f, 1.000f);
const ImVec4 COLOR_INVIS                  (0.000f, 0.000f, 0.000f, 0.000f);

void CustomHudWindowMeters::BeforeDraw()
{
}

void CustomHudWindowMeters::Draw()
{
	//set colors
	if (is_astral_available)
	{
		ImGui::PushStyleColor(ImGuiCol_Border, COLOR_ASTRAL_AVAILABLE);
		ImGui::PushStyleColor(ImGuiCol_BorderShadow, COLOR_ASTRAL_AVAILABLE);
	}

	ImVec4 colorCrossBar = COLOR_CROSS_BAR_DEFAULT;
	ImVec4 colorSkillBar = COLOR_SKILL_BAR_DEFAULT;

	const bool isBlazeActive = m_meterData->is_blaze_active;
	const bool isBlazeAvailable = m_meterData->is_blaze_available;

	if (isBlazeActive)
	{
		colorCrossBar = COLOR_CROSS_BAR_BLAZE_ACTIVE;
		colorSkillBar = COLOR_SKILL_BAR_BLAZE_ACTIVE;
	}
	else if (isBlazeAvailable)
	{
		CalculateCrossBarBlazeAvailableColor(colorCrossBar);
	}

	if (m_isRightSide)
	{
		char blaze_lvl[8];
		char skill_lvl[8];
		sprintf(blaze_lvl, "Lv %d", blaze_to_single_digit(cur_blaze_val));
		sprintf(skill_lvl, "%d", total_skill_to_single_digit(cur_skill_val));

		///////////////// POSITIONING
		ImGui::TextColored(COLOR_INVIS, blaze_lvl); //invis
		ImGui::SameLine();

		//push styles
		ImGui::PushStyleColor(ImGuiCol_Border, COLOR_INVIS); //invis
		ImGui::PushStyleColor(ImGuiCol_FrameBg, COLOR_INVIS); //invis

		//invis box
		ImGui::ColoredProgressBar(1.0f, m_skillBarSize, COLOR_INVIS, nullptr, false);

		ImGui::PopStyleColor(2);

		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() - ImGui::CalcTextSize(skill_lvl).x - (ImGui::GetStyle().ItemSpacing.x * 2));
		//////////////////////////

		/////////// SKILL BAR
		ImGui::TextColored(colorSkillBar, skill_lvl);
		ImGui::SameLine();
		DrawSkillBar(total_skill_to_single_bar_percent(cur_skill_val, isBlazeActive), colorSkillBar);

		////////// BLAZE VAL
		ImGui::TextColored(blaze_val_color, blaze_lvl);
		ImGui::SameLine();

		DrawCrossBar(total_cross_to_bar_percent(cur_cross_val), colorCrossBar);
	}
	else
	{
		/////////// SKILL BAR
		DrawSkillBar(total_skill_to_single_bar_percent(cur_skill_val, isBlazeActive), colorSkillBar);

		ImGui::SameLine();
		ImGui::TextColored(colorSkillBar, "%d", total_skill_to_single_digit(cur_skill_val));

		////////// CROSS BAR
		DrawCrossBar(total_cross_to_bar_percent(cur_cross_val), colorCrossBar);

		////////// BLAZE VAL
		ImGui::SameLine();
		ImGui::TextColored(COLOR_BLAZE_VALUE, "Lv %d", blaze_to_single_digit(cur_blaze_val));
	}

	if (is_astral_available)
		ImGui::PopStyleColor(2);
}

void CustomHudWindowMeters::AfterDraw()
{
}

void CustomHudWindowMeters::CalculateCrossBarBlazeAvailableColor(ImVec4 & colorCrossBar) const
{
	static float progressValue = 0.0f;
	static float progressDirection = 1.0f;
	const float blinkSpeed = 2.0f;
	progressValue += progressDirection * blinkSpeed * ImGui::GetIO().DeltaTime;
	if (progressValue >= +1.1f) { progressValue = +1.1f; progressDirection *= -1.0f; }
	if (progressValue <= -0.1f) { progressValue = -0.1f; progressDirection *= -1.0f; }

	colorCrossBar.y = progressValue;
}

void CustomHudWindowMeters::DrawCrossBar(const float crossPercentage, const ImVec4 & color) const
{
	ImGui::ColoredProgressBar(crossPercentage, m_crossBarSize, color, nullptr, false, m_isRightSide);
}

void CustomHudWindowMeters::DrawSkillBar(const float skillPercentage, const ImVec4 & color) const
{
	ImGui::ColoredProgressBar(skillPercentage, m_skillBarSize, color, nullptr, false, m_isRightSide);
}
