#pragma once
#include <stdint.h>

class CharInfo
{
public:
	char pad_0000[16]; //0x0000
	int32_t unknownStatus1; //0x0010
	char pad_0014[32]; //0x0014
	uint32_t char_index; //0x0034
	char pad_0038[20]; //0x0038
	class JonbEntry* pJonbEntryBegin; //0x004C
	char pad_0050[68]; //0x0050
	uint32_t hurtboxCount; //0x0094
	uint32_t hitboxCount; //0x0098
	char pad_009C[60]; //0x009C
	char* current_sprite; //0x00D8
	char pad_00DC[204]; //0x00DC
	uint32_t chie_charge_lvl; //0x01A8
	char pad_01AC[200]; //0x01AC
	int32_t facingLeft; //0x0274
	int32_t position_x; //0x0278
	int32_t position_y; //0x027C
	char pad_0280[4]; //0x0280
	int32_t offsetX_1; //0x0284
	char pad_0288[4]; //0x0288
	int32_t rotationDegrees; //0x028C
	char pad_0290[4]; //0x0290
	int32_t scaleX; //0x0294
	int32_t scaleY; //0x0298
	char pad_029C[80]; //0x029C
	int32_t position_x_dupe; //0x02EC
	int32_t position_y_dupe; //0x02F0
	char pad_02F4[16]; //0x02F4
	int32_t offsetX_2; //0x0304
	char pad_0308[4]; //0x0308
	int32_t offsetY_2; //0x030C
	char pad_0310[72]; //0x0310
	int32_t stageEdgeTouchTimer; //0x0358
	char pad_035C[888]; //0x035C
	uint32_t deny_opponent_burst; //0x06D4
	char pad_06D8[972]; //0x06D8
	uint32_t cur_hp; //0x0AA4
	uint32_t max_hp; //0x0AA8
	char pad_0AAC[7504]; //0x0AAC
	uint32_t is_char_active2; //0x27FC
	char pad_2800[4]; //0x2800
	uint32_t recoverable_hp; //0x2804
	uint32_t is_char_active; //0x2808
	char pad_280C[116580]; //0x280C
	uint32_t naoto_is_enemy_marked; //0x1EF70
	char pad_1EF74[8540]; //0x1EF74
	uint32_t unique_meter_max_val; //0x210D0
	uint32_t unique_meter_cur_val; //0x210D4
	char pad_210D8[40]; //0x210D8
}; //Size: 0x21100
