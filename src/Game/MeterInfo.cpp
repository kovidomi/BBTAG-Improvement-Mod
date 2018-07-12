#include "MeterInfo.h"

const char* meterCharNames[TOTAL_CHAR_INDEXES]
{
	"Ragna",	// 0,
	"Jin",		// 1,
	"Noel",		// 2,
	"Ra",	// 3,
	"Tager",	// 4,
	"Hakumen",	// 5,
	"Nu",		// 6,
	"Hazama",	// 7,
	"Ma",	// 8,
	"Pl", // 9,
	"Izayoi",	// 10,
	"Azrael",	// 11,
	"Nine",		// 12,
	"Es",		// 13,
	"Mai",		// 14,
	"Jubei",	// 15,
	"Yu Narukami",		// 16,
	"Yosuke",	// 17,
	"Ch",		// 18,
	"Yu",	// 19,
	"Kanji",	// 20,
	"Na",	// 21,
	"Mitsuru",	// 22,
	"Akihiko",	// 23,
	"Ae",	// 24,
	"Labrys",	// 25,
	"Hyde",		// 26,
	"Linne",	// 27,
	"Waldstein",// 28,
	"Carmine",	// 29,
	"Orie",		// 30,
	"Gordeau",	// 31,
	"Merkava",	// 32,
	"Vatista",	// 33,
	"Yuzuriha",	// 34,
	"Mika",		// 35,
	"Ruby",		// 36,
	"Weiss",	// 37,
	"Blake",	// 38,
	"Yang",		// 39,
	"Boss",		// 40
};

ImVec4 meter_color_platinum(1.0f, 0.186f, 0.713f, 1.0f);
ImVec4 meter_color_naoto(1.0f, 0.0f, 0.0f, 1.0f);
ImVec4 meter_color_aegis(0.961f, 0.668f, 0.250f, 1.0f);
ImVec4 meter_color_aegis_recover(0.985f, 0.155f, 0.155f, 1.0f);
ImVec4 meter_color_chie_lvl1(0.147f, 1.0f, 0.147f, 1.0f);
ImVec4 meter_color_chie_lvl2(1.0f, 1.0f, 0.147f, 1.0f);
ImVec4 meter_color_chie_lvl3(1.0f, 0.147f, 0.147f, 1.0f);
ImVec4 meter_color_rachel(0.961f, 0.683f, 0.953f, 1.0f);
ImVec4 meter_color_rachel_recover(0.662f, 0.000f, 0.000f, 1.000f);
ImVec4 meter_color_yukiko(0.706f, 0.0f, 0.0f, 1.0f);
ImVec4 meter_color_makoto(0.282f, 0.260f, 1.000f, 1.000f);
ImVec4 meter_color_makoto_full(0.828f, 0.025f, 1.000f, 1.000f);
ImVec4 meter_color_default(1.0f, 1.0f, 1.0f, 1.0f);

ImVec4 get_char_unique_meter_color(const CharInfo& charData)
{
	ImVec4 ret;

	switch (charData.char_index)
	{
	case CharIndex_Aegis:
		ret = meter_color_aegis;
		break;
	case CharIndex_Rachel:
		ret = meter_color_rachel;
		if (charData.unique_meter_cur_val < charData.unique_meter_max_val)
			ret = meter_color_rachel_recover;
		break;
	case CharIndex_Yukiko:
		ret = meter_color_yukiko;
		break;
	case CharIndex_Platinum:
		ret = meter_color_platinum;
		break;
	case CharIndex_Naoto:
		ret = meter_color_naoto;
		break;
	case CharIndex_Chie:
		switch (charData.chie_charge_lvl)
		{
		case 2:
			ret = meter_color_chie_lvl2;
			break;
		case 3:
			ret = meter_color_chie_lvl3;
			break;
		case 1:
		default:
			ret = meter_color_chie_lvl1;
			break;
		}
		break;
	case CharIndex_Makoto:
		if (charData.unique_meter_cur_val > 15)
			ret = meter_color_makoto_full;
		else
			ret = meter_color_makoto;
		break;
	default:
		ret = meter_color_default;
	}

	return ret;
}

int blaze_to_single_digit(int blaze_value)
{
	if (blaze_value < BLAZE_GAUGE_GAIN_STAR2)
		return 1;
	else if (blaze_value >= BLAZE_GAUGE_GAIN_STAR2 && blaze_value < BLAZE_GAUGE_GAIN_STAR3)
		return 2;
	else if (blaze_value >= BLAZE_GAUGE_GAIN_STAR3 && blaze_value < BLAZE_GAUGE_MAX_VALUE)
		return 3;
	else //BLAZE_GAUGE_MAX_VALUE
		return 4;
}

int total_skill_to_single_bar_value(int skill_value)
{
	if (skill_value < SKILL_GAUGE_DIVIDER)
		return skill_value;
	else if (skill_value == SKILL_GAUGE_MAX_VALUE || skill_value == SKILL_GAUGE_MAX_VALUE_BLAZE_ACTIVE)
		return SKILL_GAUGE_DIVIDER;

	return skill_value % SKILL_GAUGE_DIVIDER;
}
float total_skill_to_single_bar_percent(int skill_value, bool is_blaze_active)
{
	int cur_single_bar_val = total_skill_to_single_bar_value(skill_value);

	//meter is filled due to activated blaze, return a full gauge
	if (cur_single_bar_val == 0 && is_blaze_active)
		return 1.0f;

	if (cur_single_bar_val == 0)
		return 0.0f;

	return (float)cur_single_bar_val / (float)SKILL_GAUGE_DIVIDER;
}

int total_skill_to_single_digit(int skill_value)
{
	if (skill_value == 0)
		return 0;

	return skill_value / SKILL_GAUGE_DIVIDER;
}

float total_cross_to_bar_percent(int cross_value)
{
	if (cross_value == 0)
		return 0.0f;

	return (float)cross_value / (float)CROSS_GAUGE_MAX_VALUE;
}

bool show_char_unique_meter_num(const CharInfo& charData)
{
	//always show
	if (charData.char_index == CharIndex_Yukiko)
		return true;

	//only show something for naoto if she has marked someone for death
	if (charData.char_index == CharIndex_Naoto && charData.naoto_is_enemy_marked)
		return true;

	//only show if chie has charge lvl higher than 0
	if (charData.char_index == CharIndex_Chie && charData.unique_meter_cur_val > 0)
	{
		return true;
	}

	return false;
}

bool show_char_unique_meter_bar(const CharInfo& charData)
{
	bool ret = false;

	//always show 
	switch (charData.char_index)
	{
	case CharIndex_Aegis:
	case CharIndex_Rachel:
	case CharIndex_Yukiko:
		ret = true;
	}

	//only show if their meter is not null
	if ((charData.char_index == CharIndex_Chie ||
		charData.char_index == CharIndex_Platinum ||
		charData.char_index == CharIndex_Makoto) &&
		charData.unique_meter_cur_val > 0)
	{
		ret = true;
	}

	return ret;
}

bool char_has_unique_meter(CharIndex index)
{
	switch (index)
	{
	case CharIndex_Aegis:
	case CharIndex_Rachel:
	case CharIndex_Yukiko:
	case CharIndex_Chie:
	case CharIndex_Platinum:
	case CharIndex_Naoto:
	case CharIndex_Makoto:
		return true;
	default:
		return false;
	}
}