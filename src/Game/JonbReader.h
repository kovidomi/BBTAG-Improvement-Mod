#pragma once
#include "CharInfo.h"
#include "JonbEntry.h"

#include <vector>

class JonbReader
{
public:
	static std::vector<JonbEntry> getJonbEntries(const CharInfo* charObj);
};
