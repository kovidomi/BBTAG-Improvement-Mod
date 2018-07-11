#pragma once
#include "charobj.h"
#include <stdint.h>
#include <imgui.h>

#define SKILL_GAUGE_DIVIDER		5000
#define SKILL_GAUGE_MAX_VALUE	25000
#define SKILL_GAUGE_MAX_VALUE_BLAZE_ACTIVE	45000
#define CROSS_GAUGE_MAX_VALUE	10000
#define BLAZE_GAUGE_GAIN_STAR2	34000
#define BLAZE_GAUGE_GAIN_STAR3	67000
#define BLAZE_GAUGE_MAX_VALUE	100000

extern char* metercharnames[TOTAL_CHAR_INDEXES];

int		blaze_to_single_digit(int blaze_value);
//helps to fill up the skill gauge several times, like the original HUD does
int		total_skill_to_single_bar_value(int skill_value);
float	total_skill_to_single_bar_percent(int skill_value, bool is_blaze_active);
int		total_skill_to_single_digit(int skill_value);
float	total_cross_to_bar_percent(int cross_value);
bool	show_char_unique_meter_num(CharObj* CharObjInstance);
bool	show_char_unique_meter_bar(CharObj* CharObjInstance);
bool	char_has_unique_meter(CharIndex index);
ImVec4	get_char_unique_meter_color(CharObj *CharObjInstance);

class PlayerMeters
{
public:
	uint32_t P1_cur_skill; //0x0000
	uint32_t P2_cur_skill; //0x0004
	uint32_t P1_cur_cross; //0x0008
	uint32_t P2_cur_cross; //0x000C
	char pad_0010[16]; //0x0010
	uint32_t P1_cur_blaze; //0x0020
	uint32_t P2_cur_blaze; //0x0024
	uint32_t P1_is_blaze_available; //0x0028
	uint32_t P2_is_blaze_available; //0x002C
	uint32_t P1_is_blaze_active; //0x0030
	uint32_t P2_is_blaze_active; //0x0034
	uint32_t P1_last_active_blaze_lvl; //0x0038
	uint32_t P2_last_active_blaze_lvl; //0x003C
	char pad_0040[32]; //0x0040
	uint32_t P1_doing_partner_atk; //0x0060
	uint32_t P2_doing_partner_atk; //0x0064
	char pad_0068[32]; //0x0068
	uint32_t P1_is_blaze_available_2; //0x0088
	uint32_t P2_is_blaze_available_2; //0x008C
	char pad_0090[56]; //0x0090
}; //Size: 0x00C8

//for player2 we put this interface on base + 4bytes offset
class PlayerMeters_new
{
public:
	uint32_t cur_skill; //0x0000
	char unused_1[4]; //0x0004
	uint32_t cur_cross; //0x0008
	char unused_2[4]; //0x000C
	char pad_0010[16]; //0x0010
	uint32_t cur_blaze; //0x0020
	char unused_3[4]; //0x0024
	uint32_t is_blaze_available; //0x0028
	char unused_4[4]; //0x002C
	uint32_t is_blaze_active; //0x0030
	char unused_5[4]; //0x0034
	uint32_t last_active_blaze_lvl; //0x0038
	char unused_6[4]; //0x003C
	char pad_0040[32]; //0x0040
	uint32_t doing_partner_atk; //0x0060
	char unused_7[4]; //0x0064
	char pad_0068[32]; //0x0068
	uint32_t is_blaze_available_2; //0x0088
	char unused_8[4]; //0x008C
	char pad_0090[56]; //0x0090
}; //Size: 0x00C8

//in practice this is how the memory looks like:
//class PlayerMeters
//{
//public:
//	uint32_t P1_cur_skill; //0x0000
//	uint32_t P2_cur_skill; //0x0004
//	uint32_t P1_cur_cross; //0x0008
//	uint32_t P2_cur_cross; //0x000C
//	char pad_0010[16]; //0x0010
//	uint32_t P1_cur_blaze; //0x0020
//	uint32_t P2_cur_blaze; //0x0024
//	uint32_t P1_is_blaze_available; //0x0028
//	uint32_t P2_is_blaze_available; //0x002C
//	uint32_t P1_is_blaze_active; //0x0030
//	uint32_t P2_is_blaze_active; //0x0034
//	uint32_t P1_last_active_blaze_lvl; //0x0038
//	uint32_t P2_last_active_blaze_lvl; //0x003C
//	char pad_0040[32]; //0x0040
//	uint32_t P1_doing_partner_atk; //0x0060
//	uint32_t P2_doing_partner_atk; //0x0064
//	char pad_0068[32]; //0x0068
//	uint32_t P1_is_blaze_available_2; //0x0088
//	uint32_t P2_is_blaze_available_2; //0x008C
//	char pad_0090[56]; //0x0090
//}; //Size: 0x00C8