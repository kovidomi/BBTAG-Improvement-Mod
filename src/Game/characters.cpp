#include "characters.h"
#include <vector>

const std::vector<std::string> charNames
{
	"Ragna",	// 0,
	"Jin",		// 1,
	"Noel",		// 2,
	"Rachel",	// 3,
	"Tager",	// 4,
	"Hakumen",	// 5,
	"Nu",		// 6,
	"Hazama",	// 7,
	"Makoto",	// 8,
	"Platinum", // 9,
	"Izayoi",	// 10,
	"Azrael",	// 11,
	"Nine",		// 12,
	"Es",		// 13,
	"Mai",		// 14,
	"Jubei",	// 15,
	"Yu",		// 16,
	"Yosuke",	// 17,
	"Chie",		// 18,
	"Yukiko",	// 19,
	"Kanji",	// 20,
	"Naoto",	// 21,
	"Mitsuru",	// 22,
	"Akihiko",	// 23,
	"Aegis",	// 24,
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
	"NaotoKurogane", // 40,
	"Teddie",	// 41,
	"Seth",		// 42,
	"Heart",	// 43,
	"Celica",	// 44,
	"Susanoo",	// 45,
	"Adachi",	// 46,
	"Elizabeth",// 47,
	"Akatsuki",	// 48,
	"Hilda",	// 49,
	"Neo",		// 50,
	"Yumi",		// 51,
	"Blitztank",// 52,
	"Boss",		// 53,
};

int getCharactersCount()
{
	return charNames.size();
}

std::string getCharacterNameByIndexA(int charIndex)
{
	if (charIndex < getCharactersCount())
		return charNames[charIndex];

	return "Unknown";
}

std::wstring getCharacterNameByIndexW(int charIndex)
{
	std::string charName = getCharacterNameByIndexA(charIndex);
	return std::wstring(charName.begin(), charName.end());
}

bool isCharacterIndexOutOfBound(int charIndex)
{
	const int charactersCountWithoutBoss = getCharactersCount() - 1;

	if (charIndex < charactersCountWithoutBoss || charactersCountWithoutBoss < charIndex)
	{
		return true;
	}

	return false;
}
