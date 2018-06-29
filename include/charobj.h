#pragma once
#include <stdint.h>

#define TOTAL_CHAR_INDEXES 41

enum CharIndex
{
	Ragna		= 0,
	Jin			= 1,
	Noel		= 2,
	Rachel		= 3,
	Tager		= 4,
	Hakumen		= 5,
	Nu			= 6,
	Hazama		= 7,
	Makoto		= 8,
	Platinum	= 9,
	Izayoi		= 10,
	Azrael		= 11,
	Nine		= 12,
	Es			= 13,
	Mai			= 14,
	Jubei		= 15,
	Yu			= 16,
	Yosuke		= 17,
	Chie		= 18,
	Yukiko		= 19,
	Kanji		= 20,
	Naoto		= 21,
	Mitsuru		= 22,
	Akihiko		= 23,
	Aegis		= 24,
	Labrys		= 25,
	Hyde		= 26,
	Linne		= 27,
	Waldstein	= 28,
	Carmine		= 29,
	Orie		= 30,
	Gordeau		= 31,
	Merkava		= 32,
	Vatista		= 33,
	Yuzuriha	= 34,
	Mika		= 35,
	Ruby		= 36,
	Weiss		= 37,
	Blake		= 38,
	Yang		= 39,
	Boss		= 40,
};

extern char* charnames[TOTAL_CHAR_INDEXES];

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