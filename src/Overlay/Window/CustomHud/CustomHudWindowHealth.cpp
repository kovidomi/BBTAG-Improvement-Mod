#include "CustomHudWindowHealth.h"

#include "Game/characters.h"

const ImVec4 COLOR_HEALTH_BAR_FULL        (0.000f, 0.850f, 1.000f, 1.000f);
const ImVec4 COLOR_HEALTH_BAR_INJURED     (0.100f, 1.000f, 0.000f, 1.000f);
const ImVec4 COLOR_HEALTH_BAR_DANGER      (1.000f, 0.250f, 0.000f, 1.000f);
const ImVec4 COLOR_HEALTH_BAR_RECOVERABLE (0.700f, 0.000f, 0.000f, 0.750f);

void CustomHudWindowHealth::Draw()
{
	DrawHealthBarRecoverable();

	// Positioning the cursor so we render the next bar (current hp) on top of the previous one (recoverable hp)
	const ImVec2 healthBarSize = ImGui::GetItemRectSize();
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - healthBarSize.y - ImGui::GetStyle().ItemSpacing.y);

	DrawHealthBarCurrent();

	// Save cursor position to set it back here after drawing the character name, so we can continue drawing
	// the second character's bar from here
	const ImVec2 savedCursorPosition = ImGui::GetCursorPos();

	DrawCharacterName(healthBarSize);

	ImGui::SetCursorPos(savedCursorPosition);
}

ImVec4 CustomHudWindowHealth::CalculateCurrentHealthBarColor(float currentHealthPercentage) const
{
	ImVec4 healthColor;
	if (currentHealthPercentage == 1.0f)
	{
		healthColor = COLOR_HEALTH_BAR_FULL;
	}
	else if (0.35f < currentHealthPercentage && currentHealthPercentage < 1.0f)
	{
		healthColor = COLOR_HEALTH_BAR_INJURED;
	}
	else
	{
		healthColor = COLOR_HEALTH_BAR_DANGER;
	}

	return healthColor;
}

void CustomHudWindowHealth::DrawHealthBar(float healthPercentage, const ImVec4 & color) const
{
	ImGui::ColoredProgressBar(healthPercentage, m_healthBarSize, color, nullptr, false, m_isRightSide);
}

void CustomHudWindowHealth::DrawHealthBarRecoverable() const
{
	const float recoverableHealthPercent =
		(float)(m_pCharObj->recoverable_hp + m_pCharObj->cur_hp) / (float)m_pCharObj->max_hp;

	DrawHealthBar(recoverableHealthPercent, COLOR_HEALTH_BAR_RECOVERABLE);
}

void CustomHudWindowHealth::DrawHealthBarCurrent() const
{
	const float currentHealthPercent = (float)m_pCharObj->cur_hp / (float)m_pCharObj->max_hp;
	const ImVec4 healthBarColor = CalculateCurrentHealthBarColor(currentHealthPercent);
	DrawHealthBar(currentHealthPercent, healthBarColor);
}

void CustomHudWindowHealth::DrawCharacterName(const ImVec2& healthBarSize) const
{
	const char* charname = GetCharacterName();
	SetCursorPosForCharacterName(charname, healthBarSize);
	ImGui::Text("%s", charname);
}

const char * CustomHudWindowHealth::GetCharacterName() const
{
	const char* charname;
	const int charIndex = m_pCharObj->char_index;

	if (charIndex < CHAR_NAMES_COUNT - 1)
	{
		charname = charNames[charIndex].c_str();
	}
	else
	{
		charname = "<UNKNOWN>";
	}

	return charname;
}

void CustomHudWindowHealth::SetCursorPosForCharacterName(const char * charname, const ImVec2& healthBarSize) const
{
	const ImVec2 nameSize = ImGui::CalcTextSize(charname);
	ImVec2 newPos { 
		ImGui::GetCursorPosX(),
		ImGui::GetCursorPosY() - healthBarSize.y - ImGui::GetStyle().ItemSpacing.y
	};

	if (m_isRightSide)
	{
		newPos.x = ImGui::GetCursorPosX() + healthBarSize.x - nameSize.x;
	}

	ImGui::SetCursorPos(newPos);
}
