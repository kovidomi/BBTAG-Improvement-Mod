#include "url_downloader.h"
#include "../WindowManager/WindowManager.h"
#include <wininet.h>

#pragma comment(lib,"wininet.lib")

std::string DownloadUrl(std::wstring& wUrl)
{
	std::string url(wUrl.begin(), wUrl.end());

	HINTERNET connect = InternetOpen(L"MyBrowser", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);

	if (!connect)
	{
		WindowManager::AddLog("[error] DownloadUrl failed. Connection Failed or Syntax error with URL\n'%s'\n", url.c_str());
		return "";
	}

	HINTERNET OpenAddress = InternetOpenUrl(connect, wUrl.c_str(), NULL, 0, INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_KEEP_CONNECTION, 0);

	if (!OpenAddress)
	{
		DWORD ErrorNum = GetLastError();
		WindowManager::AddLog("[error] DownloadUrl failed. Failed to open URL\n'%s'\ncode: %d\n", url.c_str(), ErrorNum);
		InternetCloseHandle(connect);
		return "";
	}

	std::string receivedData = "";

	char DataReceived[4096];
	DWORD NumberOfBytesRead = 0;
	while (InternetReadFile(OpenAddress, DataReceived, 4096, &NumberOfBytesRead) && NumberOfBytesRead)
	{
		receivedData.append(DataReceived, NumberOfBytesRead);
	}

	InternetCloseHandle(OpenAddress);
	InternetCloseHandle(connect);

	return receivedData;
}