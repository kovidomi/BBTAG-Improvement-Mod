#include "MeterInfo.h"

const std::vector<std::string> meterCharNames
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
	"Ni",		// 12,
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
	"Ak",	// 23,
	"Ae",	// 24,
	"La",	// 25,
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
ImVec4 meter_color_labrys_lvl1(0.647f, 0.647f, 0.647f, 1.000f);
ImVec4 meter_color_labrys_lvl2(0.000f, 0.048f, 0.824f, 1.000f);
ImVec4 meter_color_labrys_lvl3(0.262f, 0.892f, 0.000f, 1.000f);
ImVec4 meter_color_labrys_lvl4(0.980f, 0.952f, 0.000f, 1.000f);
ImVec4 meter_color_labrys_lvl5(0.980f, 0.000f, 0.000f, 1.000f);
ImVec4 meter_color_nine_below_half(0.603f, 0.600f, 0.600f, 1.000f);
ImVec4 meter_color_nine_above_half(1.000f, 0.941f, 0.000f, 1.000f);
ImVec4 meter_color_nine_full(1.000f, 0.000f, 0.971f, 1.000f);
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
	case CharIndex_Akihiko:
		ret = meter_color_default;
		break;
	case CharIndex_Nine:
		if (charData.unique_meter_cur_val < 5000)
			ret = meter_color_nine_below_half;
		else if (charData.unique_meter_cur_val >= 5000 && charData.unique_meter_cur_val < 10000)
			ret = meter_color_nine_above_half;
		else // charData.unique_meter_cur_val >= 10000
			ret = meter_color_nine_full;
		break;
	case CharIndex_Labrys:
		if (charData.unique_meter_cur_val < 5000)
			ret = meter_color_labrys_lvl1;
		else if (charData.unique_meter_cur_val >= 5000 && charData.unique_meter_cur_val < 10000)
			ret = meter_color_labrys_lvl2;
		else if (charData.unique_meter_cur_val >= 10000 && charData.unique_meter_cur_val < 15000)
			ret = meter_color_labrys_lvl3;
		else if (charData.unique_meter_cur_val >= 15000 && charData.unique_meter_cur_val < 20000)
			ret = meter_color_labrys_lvl4;
		else // charData.unique_meter_cur_val > 20000
			ret = meter_color_labrys_lvl5;
		break;
	default:
		ret = meter_color_default;
	}

	return ret;
}

bool show_char_unique_meter_num(const CharInfo& charData)
{
	//always show
	if (charData.char_index == CharIndex_Yukiko)
		return true;

	//only show something for naoto if she has marked someone for death
	if (charData.char_index == CharIndex_Naoto && charData.naoto_is_enemy_marked)
		return true;

	//only show if chie or akihiko has charge lvl higher than 0
	if ((charData.char_index == CharIndex_Chie || charData.char_index == CharIndex_Akihiko) && 
		charData.unique_meter_cur_val > 0)
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
	case CharIndex_Nine:
	case CharIndex_Labrys:
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
	case CharIndex_Akihiko:
	case CharIndex_Nine:
	case CharIndex_Labrys:
		return true;
	default:
		return false;
	}
}