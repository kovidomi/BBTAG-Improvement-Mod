#pragma once
#include <stdint.h>

// For player2 the base address needs to be offset by 4 bytes
class MeterInfo
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
