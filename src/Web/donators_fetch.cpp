#include "donators_fetch.h"
#include "url_downloader.h"
#include "../WindowManager/WindowManager.h"
#include "../PaletteManager/impl_format.h"
#include "../logger.h"
#include <sstream>

std::vector<std::string> donators;

std::vector<std::string>& GetDonators()
{
	return donators;
}

std::vector<std::wstring> DownloadPaletteFileList()
{
	LOG(2, "DownloadPaletteFileList\n");
	WindowManager::AddLog("[system] Getting online palette list...\n");

	std::wstring wUrl = MOD_LINK_DONATORS_PALETTELIST;
	std::string data = DownloadUrl(wUrl);

	std::vector<std::wstring> links;

	if (strcmp(data.c_str(), "") != 0 && data.find("404: Not Found") == std::string::npos)
	{
		std::stringstream ss(data);
		std::string line;
		while (std::getline(ss, line, '\n'))
		{
			int pos = line.find('\r');
			if (pos != std::string::npos)
			{
				line.erase(pos);
			}

			links.push_back(std::wstring(line.begin(), line.end()));
		}
	}

	return links;
}

void DownloadPaletteFiles()
{
	std::vector<std::wstring> links = DownloadPaletteFileList();

	for (auto palUrl : links)
	{
		int bufSize = sizeof(IMPL_t);
		IMPL_t* implFile = new IMPL_t;

		int res = DownloadUrlBinary(palUrl, implFile, bufSize);

		if (res > 0)
		{
			WindowManager::AddLog("[system] Downloaded '%s'\n", implFile->paldata.palname);
			LOG(2, "Downloaded '%s'\n", implFile->paldata.palname);
			g_interfaces.pPaletteManager->LoadImplFile(*implFile);
		}

		delete implFile;
	}
}

void FetchDonators()
{
	DownloadPaletteFiles();

	std::wstring wUrl = MOD_LINK_DONATORS;
	std::string data = DownloadUrl(wUrl);

	if (strcmp(data.c_str(), "") != 0 && data.find("404: Not Found") == std::string::npos)
	{
		std::stringstream ss(data);
		std::string line;
		while (std::getline(ss, line, '\n')) 
		{
			int pos = line.find('\r');
			if (pos != std::string::npos)
			{
				line.erase(pos);
			}

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
