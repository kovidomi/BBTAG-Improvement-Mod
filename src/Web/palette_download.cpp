#include "palette_download.h"
#include "url_downloader.h"
#include "../WindowManager/WindowManager.h"
#include "../logger.h"
#include <vector>
#include <sstream>
#include <zip.h>

void Uncompress(void *data, size_t size)
{
	zip_source_t *src;
	zip_t *za;
	zip_error_t error;

	zip_error_init(&error);
	/* create source from buffer */
	if ((src = zip_source_buffer_create(data, size, 1, &error)) == NULL)
	{
		fprintf(stderr, "can't create source: %s\n", zip_error_strerror(&error));
		free(data);
		zip_error_fini(&error);
		return;
	}

	/* open zip archive from source */
	if ((za = zip_open_from_source(src, 0, &error)) == NULL)
	{
		fprintf(stderr, "can't open zip from source: %s\n", zip_error_strerror(&error));
		zip_source_free(src);
		zip_error_fini(&error);
		return;
	}
	zip_error_fini(&error);

	/* we'll want to read the data back after zip_close */
	//zip_source_keep(src);

	///* modify archive */
	//modify_archive(za);

	/* close archive */
	if (zip_close(za) < 0)
	{
		fprintf(stderr, "can't close zip archive %s\n", zip_strerror(za));
		return;
	}

	//struct zip_stat st;
	//zip_stat_init(&st);
	//zip_stat(z, name, 0, &st);

	////Alloc memory for its uncompressed contents
	//char *contents = new char[st.size];

	////Read the compressed file
	//zip_file *f = zip_fopen(z, name, 0);
	//zip_fread(f, contents, st.size);
	//zip_fclose(f);

	////And close the archive
	//zip_close(z);

	////Do something with the contents
	////delete allocated memory
	//delete[] contents;
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

