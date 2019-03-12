#include "MetersWindow.h"

#include "Core/interfaces.h"
#include "Game/MeterInfo.h"

const int SKILL_GAUGE_MAX_VALUE                    = 25000;
const int SKILL_GAUGE_MAX_VALUE_WHILE_BLAZE_ACTIVE = 45000;
const int CROSS_GAUGE_MAX_VALUE                    = 10000;
const int BLAZE_GAUGE_LVL_TWO                      = 34000;
const int BLAZE_GAUGE_LVL_THREE                    = 67000;
const int BLAZE_GAUGE_LVL_MAX                      = 100000;
const int SKILL_GAUGE_DIVIDER                      = 5000;

const ImVec4 COLOR_CROSS_BAR_DEFAULT      (0.150f, 1.000f, 1.000f, 1.000f);
const ImVec4 COLOR_SKILL_BAR_DEFAULT      (1.000f, 0.340f, 0.000f, 1.000f);
const ImVec4 COLOR_SKILL_BAR_BLAZE_ACTIVE (0.450f, 1.000f, 1.000f, 1.000f);
const ImVec4 COLOR_CROSS_BAR_BLAZE_ACTIVE (0.000f, 0.400f, 1.000f, 1.000f);
const ImVec4 COLOR_BLAZE_VALUE            (1.000f, 0.400f, 1.000f, 1.000f);
const ImVec4 COLOR_ASTRAL_AVAILABLE       (1.000f, 0.949f, 0.000f, 1.000f);
const ImVec4 COLOR_INVIS                  (0.000f, 0.000f, 0.000f, 0.000f);

void MetersWindow::BeforeDraw()
{
}

void MetersWindow::Draw()
{
	const bool isAstralColorOn = SetAstralAvailableColors();

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
		DrawAlignedToRight(colorSkillBar, colorCrossBar);
	}
	else
	{
		DrawAlignedToLeft(colorSkillBar, colorCrossBar);
	}

	if (isAstralColorOn)
	{
		ImGui::PopStyleColor(2);
	}
}

void MetersWindow::AfterDraw()
{
}

void MetersWindow::DrawAlignedToLeft(const ImVec4& colorSkillBar, const ImVec4& colorCrossBar) const
{
	DrawSkillBar(SkillValueToSingleBarPercent(), colorSkillBar);

	ImGui::SameLine();
	ImGui::TextColored(colorSkillBar, "%d", SkillValueToSingleDigit());

	DrawCrossBar(CrossValueToBarPercent(), colorCrossBar);

	ImGui::SameLine();
	ImGui::TextColored(COLOR_BLAZE_VALUE, "Lv %d", BlazeValueToSingleDigit());
}

void MetersWindow::DrawAlignedToRight(const ImVec4& colorSkillBar, const ImVec4& colorCrossBar) const
{
	std::string blazeLvl = "Lv ";
	blazeLvl += std::to_string(BlazeValueToSingleDigit());

	///////////////// Positioning with invisible text and bar
	ImGui::TextColored(COLOR_INVIS, blazeLvl.c_str());
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Border, COLOR_INVIS);
	ImGui::PushStyleColor(ImGuiCol_FrameBg, COLOR_INVIS);

	ImGui::ColoredProgressBar(1.0f, m_skillBarSize, COLOR_INVIS, nullptr, false);

	ImGui::PopStyleColor(2);
	ImGui::SameLine();
	///////////////// Positioning End

	std::string skillLvl = std::to_string(SkillValueToSingleDigit());
	ImGui::SetCursorPosX(ImGui::GetCursorPosX()
		- ImGui::CalcTextSize(skillLvl.c_str()).x - (ImGui::GetStyle().ItemSpacing.x * 2));

	ImGui::TextColored(colorSkillBar, skillLvl.c_str());
	ImGui::SameLine();
	DrawSkillBar(SkillValueToSingleBarPercent(), colorSkillBar);

	ImGui::TextColored(COLOR_BLAZE_VALUE, blazeLvl.c_str());
	ImGui::SameLine();

	DrawCrossBar(CrossValueToBarPercent(), colorCrossBar);
}

void MetersWindow::CalculateCrossBarBlazeAvailableColor(ImVec4 & colorCrossBar) const
{
	static float progressValue = 0.0f;
	static float progressDirection = 1.0f;
	const float blinkSpeed = 2.0f;
	progressValue += progressDirection * blinkSpeed * ImGui::GetIO().DeltaTime;
	if (progressValue >= +1.1f) { progressValue = +1.1f; progressDirection *= -1.0f; }
	if (progressValue <= -0.1f) { progressValue = -0.1f; progressDirection *= -1.0f; }

	colorCrossBar.y = progressValue;
}

int MetersWindow::BlazeValueToSingleDigit() const
{
	const int currentBlazeValue = m_meterData->cur_blaze;
	int blazeDigit;

	if (currentBlazeValue < BLAZE_GAUGE_LVL_TWO)
	{
		blazeDigit = 1;
	}
	else if (BLAZE_GAUGE_LVL_TWO <= currentBlazeValue && currentBlazeValue < BLAZE_GAUGE_LVL_THREE)
	{
		blazeDigit = 2;
	}
	else if (BLAZE_GAUGE_LVL_THREE <= currentBlazeValue && currentBlazeValue < BLAZE_GAUGE_LVL_MAX)
	{
		blazeDigit = 3;
	}
	else
	{
		// BLAZE_GAUGE_LVL_MAX
		blazeDigit = 4;
	}

	return blazeDigit;
}

int MetersWindow::SkillValueToSingleBar() const
{
	const int currentSkillValue = m_meterData->cur_skill;

	const bool isLessThanABar = currentSkillValue < SKILL_GAUGE_DIVIDER;
	const bool isBarFull = currentSkillValue == SKILL_GAUGE_MAX_VALUE
		|| currentSkillValue == SKILL_GAUGE_MAX_VALUE_WHILE_BLAZE_ACTIVE;

	if (isLessThanABar)
	{
		return currentSkillValue;
	}
	if (isBarFull)
	{
		return SKILL_GAUGE_DIVIDER;
	}

	return currentSkillValue % SKILL_GAUGE_DIVIDER;
}

float MetersWindow::SkillValueToSingleBarPercent() const
{
	if (m_meterData->is_blaze_active)
	{
		return 1.0f;
	}

	return SkillValueToSingleBar() / (float)SKILL_GAUGE_DIVIDER;
}

int MetersWindow::SkillValueToSingleDigit() const
{
	return m_meterData->cur_skill / (float)SKILL_GAUGE_DIVIDER;
}

float MetersWindow::CrossValueToBarPercent() const
{
	return m_meterData->cur_cross / (float)CROSS_GAUGE_MAX_VALUE;
}

bool MetersWindow::IsAstralAvailable(Player & opponentPlayer) const
{
	const int characterOneCurrentHp = opponentPlayer.GetChar1().GetData()->cur_hp;
	const int characterTwoCurrentHp = opponentPlayer.GetChar2().GetData()->cur_hp;
	const bool anyCharactersDead = characterOneCurrentHp <= 0 || characterTwoCurrentHp <= 0;
	const int currentSkillValue = m_meterData->cur_skill;

	if (SKILL_GAUGE_MAX_VALUE_WHILE_BLAZE_ACTIVE <= currentSkillValue && anyCharactersDead)
	{
		return true;
	}

	return false;
}

bool MetersWindow::SetAstralAvailableColors() const
{
	Player* opponentPlayer = &g_interfaces.player2;

	if (m_isRightSide)
	{
		opponentPlayer = &g_interfaces.player1;
	}

	const bool isAstralAvailable = IsAstralAvailable(*opponentPlayer);

	if (isAstralAvailable)
	{
		ImGui::PushStyleColor(ImGuiCol_Border, COLOR_ASTRAL_AVAILABLE);
		ImGui::PushStyleColor(ImGuiCol_BorderShadow, COLOR_ASTRAL_AVAILABLE);
	}

	return isAstralAvailable;
}

void MetersWindow::DrawCrossBar(float crossPercentage, const ImVec4 & color) const
{
	ImGui::ColoredProgressBar(crossPercentage, m_crossBarSize, color, nullptr, false, m_isRightSide);
}

void MetersWindow::DrawSkillBar(float skillPercentage, const ImVec4 & color) const
{
	ImGui::ColoredProgressBar(skillPercentage, m_skillBarSize, color, nullptr, false, m_isRightSide);
}
