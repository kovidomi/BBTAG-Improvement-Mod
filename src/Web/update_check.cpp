#include "update_check.h"

#include "url_downloader.h"

#include "Core/logger.h"
#include "Core/info.h"
#include "Overlay/WindowManager.h"

#include <handleapi.h>
#include <processthreadsapi.h>
#include <regex>

std::string newVersionNum = "";

std::string GetNewVersionNum()
{
	if (newVersionNum != "")
	{
		return newVersionNum;
	}
	return "";
}

void CheckUpdate()
{
	std::wstring wUrl = MOD_LINK_FORUM;
	std::string data = DownloadUrl(wUrl);

	if (strcmp(data.c_str(), "") == 0)
	{
		WindowManager::getInstance().AddLog("[error] Update check failed. No data downloaded.\n");
		LOG(2, "Update check failed.No data downloaded.\n");
		return;
	}

	data = data.substr(0, 950).c_str();

	//fits on: <title>[BBTAG IMPROVEMENT MOD] (v1.03
	//and captures: v1.03
	std::regex r("<title>.+(v\\d\.\\d\\d)");
	std::smatch m;
	std::regex_search(data, m, r);

	if (m[1].str() == "")
	{
		WindowManager::getInstance().AddLog("[error] Update check failed. Regex no match.\n");
		return;
	}

	if (strcmp(m[1].str().c_str(), MOD_VERSION_NUM) != 0)
	{
		newVersionNum = m[1].str();

		LOG(2, "New version found: %s\n", newVersionNum.c_str());
		WindowManager::getInstance().AddLog("[system] Update available: BBTAG Improvement Mod %s has been released!\n",
			newVersionNum.c_str());

		WindowManager::getInstance().SetUpdateAvailable();
	}
	else
	{
		WindowManager::getInstance().AddLog("[system] BBTAG Improvement Mod is up-to-date\n");
	}
}

void StartAsyncUpdateCheck()
{
	if (Settings::settingsIni.checkupdates)
	{
		CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)CheckUpdate, nullptr, 0, nullptr));
	}
}
