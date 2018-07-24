#include "palette_download.h"
#include "url_downloader.h"
#include "../WindowManager/WindowManager.h"
#include "../logger.h"
#include <vector>
#include <sstream>

#define ZLIB_WINAPI   // actually actually needed (for linkage)

#include "zlib.h"     // declare the external fns -- uses zconf.h, too
#pragma comment(lib, "zlibstat.lib")

int UncompressData(const BYTE* abSrc, int nLenSrc, BYTE* abDst, int nLenDst)
{
	z_stream zInfo = { 0 };
	zInfo.total_in = zInfo.avail_in = nLenSrc;
	zInfo.total_out = zInfo.avail_out = nLenDst;
	zInfo.next_in = (BYTE*)abSrc;
	zInfo.next_out = abDst;

	int nErr, nRet = -1;
	nErr = inflateInit(&zInfo);               // zlib function
	if (nErr == Z_OK) 
	{
		nErr = inflate(&zInfo, Z_FINISH);     // zlib function
		if (nErr == Z_STREAM_END) 
		{
			nRet = zInfo.total_out;
		}
	}
	inflateEnd(&zInfo);   // zlib function
	return(nRet); // -1 or len of output
}

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
	//std::vector<std::wstring> links = DownloadPaletteFileList();
	std::wstring palUrl(L"https://github.com/kovidomi/BBTAG-Improvement-Mod/raw/master/resource/palettes/pal_test.zip");
	/*for (auto palUrl : links)
	{*/
		int bufSize = sizeof(IMPL_t);
		//IMPL_t* implFile = new IMPL_t;
		char* ptr = new char[1000000];
		char* dest = new char[5000000];
		//int res = DownloadUrlBinary(palUrl, implFile, bufSize);
		int res = DownloadUrlBinary(palUrl, ptr, 1000000);

		if (res > 0)
		{
			res = UncompressData((BYTE*)ptr, res, (BYTE*)dest, 5000000);
			//WindowManager::AddLog("[system] Downloaded '%s%s'\n", implFile->paldata.palname, ".impl");
			//LOG(2, "Downloaded '%s'\n", implFile->paldata.palname);
			//g_interfaces.pPaletteManager->PushImplFileIntoVector(*implFile);
		}

		delete[] ptr;
		delete[] dest;
		//delete implFile;
	//}
}

void InitiateDownloadingPaletteFiles()
{
	HANDLE paletteDownloadThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)DownloadPaletteFiles, NULL, NULL, NULL);
	if (paletteDownloadThread)
		CloseHandle(paletteDownloadThread);
}

