#pragma once
#include <stdint.h>

class CharInfo
{
public:
	char pad_0000[52]; //0x0000
	uint32_t char_index; //0x0034
	char pad_0038[92]; //0x0038
	uint32_t total_hitboxes; //0x0094
	uint32_t total_hurtboxes; //0x0098
	char pad_009C[60]; //0x009C
	char* current_sprite; //0x00D8
	char pad_00DC[204]; //0x00DC
	uint32_t chie_charge_lvl; //0x01A8
	char pad_01AC[204]; //0x01AC
	int32_t position_x; //0x0278
	int32_t position_y; //0x027C
	char pad_0280[1108]; //0x0280
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