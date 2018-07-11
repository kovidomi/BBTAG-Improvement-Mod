#pragma once
#include <stdint.h>

#define TOTAL_CHAR_INDEXES 41

extern char* charnames[TOTAL_CHAR_INDEXES];
extern wchar_t* wcharnames[TOTAL_CHAR_INDEXES];

enum CharIndex
{
	CharIndex_Ragna		= 0,
	CharIndex_Jin		= 1,
	CharIndex_Noel		= 2,
	CharIndex_Rachel	= 3,
	CharIndex_Tager		= 4,
	CharIndex_Hakumen	= 5,
	CharIndex_Nu		= 6,
	CharIndex_Hazama	= 7,
	CharIndex_Makoto	= 8,
	CharIndex_Platinum	= 9,
	CharIndex_Izayoi	= 10,
	CharIndex_Azrael	= 11,
	CharIndex_Nine		= 12,
	CharIndex_Es		= 13,
	CharIndex_Mai		= 14,
	CharIndex_Jubei		= 15,
	CharIndex_Yu		= 16,
	CharIndex_Yosuke	= 17,
	CharIndex_Chie		= 18,
	CharIndex_Yukiko	= 19,
	CharIndex_Kanji		= 20,
	CharIndex_Naoto		= 21,
	CharIndex_Mitsuru	= 22,
	CharIndex_Akihiko	= 23,
	CharIndex_Aegis		= 24,
	CharIndex_Labrys	= 25,
	CharIndex_Hyde		= 26,
	CharIndex_Linne		= 27,
	CharIndex_Waldstein	= 28,
	CharIndex_Carmine	= 29,
	CharIndex_Orie		= 30,
	CharIndex_Gordeau	= 31,
	CharIndex_Merkava	= 32,
	CharIndex_Vatista	= 33,
	CharIndex_Yuzuriha	= 34,
	CharIndex_Mika		= 35,
	CharIndex_Ruby		= 36,
	CharIndex_Weiss		= 37,
	CharIndex_Blake		= 38,
	CharIndex_Yang		= 39,
	CharIndex_Boss		= 40,
};

class CharObj
{
public:
	char pad_0000[52]; //0x0000
	uint32_t char_index; //0x0034
	char pad_0038[368]; //0x0038
	uint32_t chie_charge_lvl; //0x01A8
	char pad_01AC[2296]; //0x01AC
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