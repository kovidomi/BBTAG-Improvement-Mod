#include "donators_fetch.h"
#include "url_downloader.h"
#include "../WindowManager/WindowManager.h"
#include "../logger.h"
#include <sstream>

std::vector<std::string> donators;

std::vector<std::string>& GetDonators()
{
	return donators;
}

void FetchDonators()
{
	std::wstring wUrl = MOD_LINK_DONATORS;
	std::string data = DownloadUrl(wUrl);
													//404: Not Found
	if (strcmp(data.c_str(), "") != 0 && data.find("404: Not Found") == std::string::npos)
	{
		std::stringstream ss(data);
		std::string line;
		while (std::getline(ss, line, '\n')) 
		{
			line.erase(0, strlen("DONATOR("));
			line.erase(line.length() - 1);
			donators.push_back(line);
		}
		return;
	}
	WindowManager::AddLog("[error] Donators fetch failed. No data downloaded.\n");
	LOG(2, "Donators fetch failed. No data downloaded.\n");

	//X-MACRO, loading donators from file
#define DONATOR(_name) \
	donators.push_back(#_name);
#include "../../resource/donators.txt"
#undef DONATOR
}
