#include "palette_download.h"
#include "url_downloader.h"
#include "../WindowManager/WindowManager.h"
#include "../logger.h"
#include <vector>
#include <sstream>

std::vector<std::wstring> DownloadPaletteFileList()
{
	LOG(2, "DownloadPaletteFileList\n");
	WindowManager::AddLog("[system] Getting online palette list...\n");

	//std::wstring wUrl = MOD_LINK_DONATORS_PALETTELIST;
	std::wstring wUrl = MOD_LINK_NORMAL_PALETTELIST;
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
	else
	{
		LOG(2, "DownloadPaletteFileList failed, no data received, or 404!!!\n");
		WindowManager::AddLog("[error] Failed to retrieve online palette list. No data, or 404\n");
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
			WindowManager::AddLog("[system] Downloaded '%s%s'\n", implFile->paldata.palname, ".impl");
			LOG(2, "Downloaded '%s'\n", implFile->paldata.palname);
			g_interfaces.pPaletteManager->PushImplFileIntoVector(*implFile);
		}

		delete implFile;
	}
}

void InitiateDownloadingPaletteFiles()
{
	HANDLE paletteDownloadThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)DownloadPaletteFiles, NULL, NULL, NULL);
	if (paletteDownloadThread)
		CloseHandle(paletteDownloadThread);
}