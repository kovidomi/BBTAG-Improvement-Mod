#include "bbtag_im.h"
#include "Hooks/hooks_importAddressTable.h"
#include "WindowManager/WindowManager.h"
#include "globals.h"
#include "crashdump.h"
#include "logger.h"
#include "settings.h"
#include "PaletteManager/PaletteManager.h"

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
		g_interfaces.pPaletteManager = new PaletteManager();
		g_interfaces.pCharPalInfos = new CharPalInfos();

		SetUnhandledExceptionFilter(UnhandledExFilter);
	}
}

void BBTAG_IM_Shutdown()
{
	WindowManager::Shutdown();

	CleanupGlobals();

	closeLogger();
}