#pragma once
#include <string>

std::string DownloadUrl(std::wstring& wUrl);

//return number of bytes read
unsigned long DownloadUrlBinary(std::wstring& wUrl, void** outBuffer);