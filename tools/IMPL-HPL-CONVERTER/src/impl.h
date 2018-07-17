#pragma once
#include "characters.h"
#include "impl_format.h"
#include <string>

IMPL_t* LoadImplFromFile(std::wstring& wFolderPath);
bool WriteImplToFile(IMPL_t *filledImpl);