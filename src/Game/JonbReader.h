#pragma once
#include "CharInfo.h"
#include "JonbEntry.h"

#include <vector>

class JonbReader
{
	static std::vector<JonbEntry> getJonbEntries(CharInfo* charObj);
};
