#include "donators_fetch.h"

#include "url_downloader.h"

#include "Core/info.h"
#include "Core/logger.h"
#include "Overlay/Logger/ImGuiLogger.h"

#include <sstream>

std::vector<std::string> donatorNames;
std::vector<int> donatorTiers;

const std::vector<std::string>& GetDonatorNames()
{
	return donatorNames;
}

const std::vector<int>& GetDonatorTiers()
{
	return donatorTiers;
}

void FetchDonators()
{
	std::wstring wUrl = MOD_LINK_DONATORS;
	std::string data = DownloadUrl(wUrl);

	//lame way of looking for proper response
	if (data.find("DONATOR(") != std::string::npos)
	{
		std::stringstream ss(data);
		std::string line;
		while (std::getline(ss, line, '\n')) 
		{
			if (line.find("DONATOR(") != std::string::npos)
			{
				int pos = line.find('\r');
				if (pos != std::string::npos)
				{
					line.erase(pos);
				}

				line.erase(0, strlen("DONATOR("));
				line.erase(line.length() - 1);

				pos = line.find(',');
				if (pos != std::string::npos)
				{
					donatorNames.push_back(line.substr(0, pos));
					std::string lastChar(&line.back());
					donatorTiers.push_back(atoi(lastChar.c_str()));
				}
			}
		}
		return;
	}
	g_imGuiLogger->Log("[error] Donators fetch failed. No data downloaded.\n");
	LOG(2, "Donators fetch failed. No data downloaded.\n");

	//X-MACRO, loading donators and tiers from file
#define DONATOR(_name, _tier) { \
	donatorNames.push_back(#_name); \
	donatorTiers.push_back(_tier); }
#include "../../resource/donators.txt"
#undef DONATOR
}

void StartAsyncDonatorsFetch()
{
	CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)FetchDonators, nullptr, 0, nullptr));
}
