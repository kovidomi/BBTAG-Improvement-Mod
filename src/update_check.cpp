#include "update_check.h"
#include "WindowManager/WindowManager.h"
#include "settings.h"
#include "logger.h"
#include <regex>
#include <cstring>
#include <wininet.h>

#pragma comment(lib,"wininet.lib")

std::string newVersionNum = "";

std::string GetNewVersionNum()
{
	if (newVersionNum != "")
		return newVersionNum;
	else
		return "";
}

void CheckUpdate()
{
	HINTERNET connect = InternetOpen(L"MyBrowser", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);

	if (!connect) 
	{
		WindowManager::AddLog("[error] Update check failed. Connection Failed or Syntax error\n");
		return;
	}

	HINTERNET OpenAddress = InternetOpenUrl(connect, MOD_LINK_FORUM, NULL, 0, INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_KEEP_CONNECTION, 0);

	if (!OpenAddress) 
	{
		DWORD ErrorNum = GetLastError();
		WindowManager::AddLog("[error] Update check failed. Failed to open URL %d\n", ErrorNum);
		InternetCloseHandle(connect);
		return;
	}

	std::string data = "";

	char DataReceived[4096];
	DWORD NumberOfBytesRead = 0;
	while (InternetReadFile(OpenAddress, DataReceived, 4096, &NumberOfBytesRead) && NumberOfBytesRead)
	{
		data.append(DataReceived, NumberOfBytesRead);
	}
	
	InternetCloseHandle(OpenAddress);
	InternetCloseHandle(connect);

	data = data.substr(0, 950).c_str();
	std::regex r("<title>.+(v\\d\.\\d\\d)"); //look for the title of the site
	std::smatch m;
	std::regex_search(data, m, r);

	if (m[1].str() == "")
	{
		WindowManager::AddLog("[error] Update check failed. Regex no match.\n");
		return;
	}

	if (strcmp(m[1].str().c_str(), MOD_VERSION_NUM) != 0)
	{
		newVersionNum = m[1].str();
		LOG(2, "New version found: %s\n", newVersionNum.c_str());
		WindowManager::AddLog("[system] Update available: BBTAG Improvement Mod %s has been released!\n", newVersionNum.c_str());
		WindowManager::IsUpdateAvailable = true;
	}
	else
	{
		WindowManager::AddLog("[system] BBTAG Improvement Mod is up-to-date\n");
	}
}