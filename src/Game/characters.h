#pragma once
#include <string>

enum CharIndex
{
	CharIndex_Ragna = 0,
	CharIndex_Jin = 1,
	CharIndex_Noel = 2,
	CharIndex_Rachel = 3,
	CharIndex_Tager = 4,
	CharIndex_Hakumen = 5,
	CharIndex_Nu = 6,
	CharIndex_Hazama = 7,
	CharIndex_Makoto = 8,
	CharIndex_Platinum = 9,
	CharIndex_Izayoi = 10,
	CharIndex_Azrael = 11,
	CharIndex_Nine = 12,
	CharIndex_Es = 13,
	CharIndex_Mai = 14,
	CharIndex_Jubei = 15,
	CharIndex_Yu = 16,
	CharIndex_Yosuke = 17,
	CharIndex_Chie = 18,
	CharIndex_Yukiko = 19,
	CharIndex_Kanji = 20,
	CharIndex_Naoto = 21,
	CharIndex_Mitsuru = 22,
	CharIndex_Akihiko = 23,
	CharIndex_Aegis = 24,
	CharIndex_Labrys = 25,
	CharIndex_Hyde = 26,
	CharIndex_Linne = 27,
	CharIndex_Waldstein = 28,
	CharIndex_Carmine = 29,
	CharIndex_Orie = 30,
	CharIndex_Gordeau = 31,
	CharIndex_Merkava = 32,
	CharIndex_Vatista = 33,
	CharIndex_Yuzuriha = 34,
	CharIndex_Mika = 35,
	CharIndex_Ruby = 36,
	CharIndex_Weiss = 37,
	CharIndex_Blake = 38,
	CharIndex_Yang = 39,
	CharIndex_NaotoKurogane = 40,
	CharIndex_Teddie = 41,
	CharIndex_Seth = 42,
	CharIndex_Heart = 43,
	CharIndex_Celica = 44,
	CharIndex_Susanoo = 45,
	CharIndex_Adachi = 46,
	CharIndex_Elizabeth = 47,
	CharIndex_Akatsuki = 48,
	CharIndex_Hilda = 49,
	CharIndex_Neo = 50,
	CharIndex_Yumi = 51,
	CharIndex_Blitztank = 52,
	CharIndex_Boss = 53
};

int getCharactersCount();

std::string getCharacterNameByIndexA(int charIndex);

std::wstring getCharacterNameByIndexW(int charIndex);

bool isCharacterIndexOutOfBound(int charIndex);
