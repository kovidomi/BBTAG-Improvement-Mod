#include "CharMetersWindow.h"

#include "Game/characters.h"

const std::vector<std::string> meterCharNames
{
	"Ragna",    // 0,
	"Jin",      // 1,
	"Noel",     // 2,
	"Ra",       // 3,
	"Tager",    // 4,
	"Hakumen",  // 5,
	"Nu",       // 6,
	"Hazama",   // 7,
	"Ma",       // 8,
	"Pl",       // 9,
	"Izayoi",   // 10,
	"Azrael",   // 11,
	"Ni",       // 12,
	"Es",       // 13,
	"Mai",      // 14,
	"Jubei",    // 15,
	"Yu Narukami",// 16,
	"Yosuke",   // 17,
	"Ch",       // 18,
	"Yu",       // 19,
	"Kanji",    // 20,
	"Na",       // 21,
	"Mitsuru",  // 22,
	"Ak",       // 23,
	"Ae",       // 24,
	"La",       // 25,
	"Hyde",	    // 26,
	"Linne",    // 27,
	"Waldstein",// 28,
	"Carmine",  // 29,
	"Orie",	    // 30,
	"Gordeau",  // 31,
	"Merkava",  // 32,
	"Vatista",  // 33,
	"Yuzuriha", // 34,
	"Mika",	    // 35,
	"Ruby",     // 36,
	"Weiss",    // 37,
	"Blake",    // 38,
	"Yang",     // 39,
	"Boss",     // 40
};

const ImVec4 COLOR_BAR_DEFAULT          (1.000f, 1.000f, 1.000f, 1.000f);
const ImVec4 COLOR_BAR_AEGIS            (0.961f, 0.668f, 0.250f, 1.000f);
const ImVec4 COLOR_BAR_AEGIS_RECOVER    (0.985f, 0.155f, 0.155f, 1.000f);
const ImVec4 COLOR_BAR_CHIE_LVL_ONE     (0.147f, 1.000f, 0.147f, 1.000f);
const ImVec4 COLOR_BAR_CHIE_LVL_TWO     (1.000f, 1.000f, 0.147f, 1.000f);
const ImVec4 COLOR_BAR_CHIE_LVL_THREE   (1.000f, 0.147f, 0.147f, 1.000f);
const ImVec4 COLOR_BAR_LABRYS_LVL_ONE   (0.647f, 0.647f, 0.647f, 1.000f);
const ImVec4 COLOR_BAR_LABRYS_LVL_TWO   (0.000f, 0.048f, 0.824f, 1.000f);
const ImVec4 COLOR_BAR_LABRYS_LVL_THREE (0.262f, 0.892f, 0.000f, 1.000f);
const ImVec4 COLOR_BAR_LABRYS_LVL_FOUR  (0.980f, 0.952f, 0.000f, 1.000f);
const ImVec4 COLOR_BAR_LABRYS_LVL_FIVE  (0.980f, 0.000f, 0.000f, 1.000f);
const ImVec4 COLOR_BAR_MAKOTO           (0.282f, 0.260f, 1.000f, 1.000f);
const ImVec4 COLOR_BAR_MAKOTO_FULL      (0.828f, 0.025f, 1.000f, 1.000f);
const ImVec4 COLOR_BAR_NAOTO            (1.000f, 0.000f, 0.000f, 1.000f);
const ImVec4 COLOR_BAR_NINE_BELOW_HALF  (0.603f, 0.600f, 0.600f, 1.000f);
const ImVec4 COLOR_BAR_NINE_ABOVE_HALF  (1.000f, 0.941f, 0.000f, 1.000f);
const ImVec4 COLOR_BAR_NINE_FULL        (1.000f, 0.000f, 0.971f, 1.000f);
const ImVec4 COLOR_BAR_PLATINUM         (1.000f, 0.186f, 0.713f, 1.000f);
const ImVec4 COLOR_BAR_RACHEL           (0.961f, 0.683f, 0.953f, 1.000f);
const ImVec4 COLOR_BAR_RACHEL_RECOVER   (0.662f, 0.000f, 0.000f, 1.000f);
const ImVec4 COLOR_BAR_YUKIKO           (0.706f, 0.000f, 0.000f, 1.000f);

const ImVec4 COLOR_TRANSPARENT          (0.000f, 0.000f, 0.000f, 0.000f);

bool CharMetersWindow::CharacterHasUniqueMeterToDraw() const
{
	switch (m_pCharObj->char_index)
	{
	case CharIndex_Aegis:
	case CharIndex_Akihiko:
	case CharIndex_Chie:
	case CharIndex_Labrys:
	case CharIndex_Makoto:
	case CharIndex_Naoto:
	case CharIndex_Nine:
	case CharIndex_Platinum:
	case CharIndex_Rachel:
	case CharIndex_Yukiko:
		return (ShouldShowUniqueMeterNumber() || ShouldShowUniqueMeterBar()) && m_pCharObj->cur_hp != 0;
	default:
		return false;
	}
}

void CharMetersWindow::Draw()
{
	if (m_isRightSide)
	{
		DrawAlignedToRight();
	}
	else
	{
		DrawAlignedToLeft();
	}
}

float CharMetersWindow::GetUniqueMeterBarPercentage() const
{
	return (float)m_pCharObj->unique_meter_cur_val / (float)m_pCharObj->unique_meter_max_val;
}

std::string CharMetersWindow::GetUniqueMeterNumberString() const
{
	std::string valueToShow = "?";
	const CharIndex charIndex = (CharIndex)m_pCharObj->char_index;
	
	if (charIndex == CharIndex_Yukiko || charIndex == CharIndex_Akihiko) 
	{
		valueToShow = std::to_string(m_pCharObj->unique_meter_cur_val);
	} 
	else if (charIndex == CharIndex_Naoto)
	{
		m_pCharObj->naoto_is_enemy_marked == 1 ? valueToShow = "X" : valueToShow = " ";
	}
	else if (charIndex == CharIndex_Chie)
	{
		valueToShow = std::to_string(m_pCharObj->chie_charge_lvl);
	}

	return valueToShow;
}

void CharMetersWindow::DrawAlignedToLeft() const
{
	ImGui::Text("%s", meterCharNames[m_pCharObj->char_index].c_str());
	ImGui::SameLine();

	if (ShouldShowUniqueMeterBar())
	{
		ImGui::ColoredProgressBar(GetUniqueMeterBarPercentage(), m_uniqueBarSize, GetBarColor(), nullptr, false);
	}
	else
	{
		ImGui::ColoredProgressBar(0.0f, m_uniqueBarSize, COLOR_TRANSPARENT, nullptr, false);
	}

	ImGui::SameLine();
	if (ShouldShowUniqueMeterNumber())
	{
		ImGui::TextColored(GetBarColor(), "%s", GetUniqueMeterNumberString().c_str());
	}
	else
	{
		ImGui::TextColored(COLOR_TRANSPARENT, "%s", "");
	}
}

void CharMetersWindow::DrawAlignedToRight() const
{
	if (ShouldShowUniqueMeterNumber())
	{
		ImGui::TextColored(GetBarColor(), "%s", GetUniqueMeterNumberString().c_str());
	}
	else  // Drawing invis num for alignment
	{
		ImGui::TextColored(COLOR_TRANSPARENT, "%s", "");
	}

	ImGui::SameLine();
	if (ShouldShowUniqueMeterBar())
	{
		ImGui::ColoredProgressBar(GetUniqueMeterBarPercentage(), m_uniqueBarSize, 
			GetBarColor(), nullptr, false, m_isRightSide);
	}
	else
	{
		ImGui::ColoredProgressBar(0.0f, m_uniqueBarSize, COLOR_TRANSPARENT, nullptr, false, m_isRightSide);
	}

	ImGui::SameLine();
	ImGui::Text("%s", meterCharNames[m_pCharObj->char_index].c_str());
}

bool CharMetersWindow::ShouldShowUniqueMeterNumber() const
{
	const CharIndex charIndex = (CharIndex)m_pCharObj->char_index;

	//Always show
	if (charIndex == CharIndex_Yukiko)
	{
		return true;
	}

	if (charIndex == CharIndex_Naoto && m_pCharObj->naoto_is_enemy_marked)
	{
		return true;
	}

	if ((charIndex == CharIndex_Chie || charIndex == CharIndex_Akihiko) 
		&& 0 < m_pCharObj->unique_meter_cur_val)
	{
		return true;
	}

	return false;
}

bool CharMetersWindow::ShouldShowUniqueMeterBar() const
{
	// Always show 
	switch (m_pCharObj->char_index)
	{
		case CharIndex_Aegis:
		case CharIndex_Labrys:
		case CharIndex_Nine:
		case CharIndex_Rachel:
		case CharIndex_Yukiko:
			{
				return true;
			}
		case CharIndex_Chie:
		case CharIndex_Makoto:
		case CharIndex_Platinum:
			if (0 < m_pCharObj->unique_meter_cur_val)
			{
				return true;
			}
		default:
			return false;
	}
}

ImVec4 CharMetersWindow::GetBarColor() const
{
	const CharIndex charIndex = (CharIndex)m_pCharObj->char_index;

	if (charIndex == CharIndex_Aegis)
	{
		return COLOR_BAR_AEGIS;
	}

	if (charIndex == CharIndex_Akihiko)
	{
		return COLOR_BAR_DEFAULT;
	}

	if (charIndex == CharIndex_Chie)
	{
		const int chieChargeLvl = m_pCharObj->chie_charge_lvl;
		if (chieChargeLvl == 2)
		{
			return COLOR_BAR_CHIE_LVL_TWO;
		}

		if (chieChargeLvl == 3)
		{
			return COLOR_BAR_CHIE_LVL_THREE;
		}

		return COLOR_BAR_CHIE_LVL_ONE;
	}

	if (charIndex == CharIndex_Labrys)
	{
		const int uniqueBarValue = m_pCharObj->unique_meter_cur_val;
		if (uniqueBarValue < 5000)
		{
			return COLOR_BAR_LABRYS_LVL_ONE;
		}

		if (5000 <= uniqueBarValue && uniqueBarValue < 10000)
		{
			return COLOR_BAR_LABRYS_LVL_TWO;
		}

		if (10000 <= uniqueBarValue && uniqueBarValue < 15000)
		{
			return COLOR_BAR_LABRYS_LVL_THREE;
		}

		if (15000 <= uniqueBarValue && uniqueBarValue < 20000)
		{
			return COLOR_BAR_LABRYS_LVL_FOUR;
		}
		// 20000 < uniqueBarValue
		return COLOR_BAR_LABRYS_LVL_FIVE;
	}

	if (charIndex == CharIndex_Makoto)
	{
		if (15 < m_pCharObj->unique_meter_cur_val)
		{
			return COLOR_BAR_MAKOTO_FULL;
		}
		return COLOR_BAR_MAKOTO;
	}

	if (charIndex == CharIndex_Naoto)
	{
		return COLOR_BAR_NAOTO;
	}

	if (charIndex == CharIndex_Nine)
	{
		const int uniqueBarValue = m_pCharObj->unique_meter_cur_val;
		if (uniqueBarValue < 5000)
		{
			return COLOR_BAR_NINE_BELOW_HALF;
		}

		if (5000 <= uniqueBarValue && uniqueBarValue < 10000)
		{
			return COLOR_BAR_NINE_ABOVE_HALF;
		}
		// 10000 <= uniqueBarValue
		return COLOR_BAR_NINE_FULL;
	}

	if (charIndex == CharIndex_Rachel)
	{
		if (m_pCharObj->unique_meter_cur_val < m_pCharObj->unique_meter_max_val)
		{
			return COLOR_BAR_RACHEL_RECOVER;
		}
		return COLOR_BAR_RACHEL;
	}

	if (charIndex == CharIndex_Platinum)
	{
		return COLOR_BAR_PLATINUM;
	}

	if (charIndex == CharIndex_Yukiko)
	{
		return COLOR_BAR_YUKIKO;
	}

	return COLOR_BAR_DEFAULT;
}
