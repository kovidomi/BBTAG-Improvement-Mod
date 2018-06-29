#include "../include/bbtag_im.h"
#include "../include/Hooks/hooks_importAddressTable.h"
#include "../include/WindowManager/WindowManager.h"
#include "../include/containers.h"
#include "../include/crashdump.h"
#include "../include/logger.h"
#include "../include/settings.h"

void CreateCustomDirectories()
{
	LOG(2, "CreateCustomDirectories\n");

	CreateDirectory(L"BBTAG_IM", NULL);
}

void BBTAG_IM_Start()
{
	if (Settings::loadSettingsFile())
	{
		logSettingsIni();
		Settings::initSavedSettings();

		if (!placeHooks_importAddressTable())
		{
			MessageBoxA(NULL, "Failed IAT hook", "BBTAGIM", MB_OK);
			BBTAG_IM_Shutdown();
			ExitProcess(0);
		}

		CreateCustomDirectories();
		SetUnhandledExceptionFilter(UnhandledExFilter);
	}
}

void BBTAG_IM_Shutdown()
{
	WindowManager::Shutdown();

	Containers::Cleanup();

	closeLogger();
}