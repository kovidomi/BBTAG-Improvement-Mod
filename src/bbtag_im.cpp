#include "bbtag_im.h"
#include "Hooks/hooks_detours.h"
#include "WindowManager/WindowManager.h"
#include "interfaces.h"
#include "crashdump.h"
#include "logger.h"
#include "Settings.h"
#include "PaletteManager/PaletteManager.h"
#include "Discord/Discord.h"

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

		if (!placeHooks_detours())
		{
			MessageBoxA(NULL, "Failed IAT hook", "BBTAGIM", MB_OK);
			BBTAG_IM_Shutdown();
			ExitProcess(0);
		}

		CreateCustomDirectories();
		g_interfaces.pPaletteManager = new PaletteManager();
		g_interfaces.pDiscord = new Discord("485863441610047488", "702890");

		SetUnhandledExceptionFilter(UnhandledExFilter);
	}
}

void BBTAG_IM_Shutdown()
{
	WindowManager::Shutdown();

	CleanupInterfaces();

	closeLogger();
}