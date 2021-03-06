#include "crashdump.h"
#include "interfaces.h"
#include "logger.h"
#include "Settings.h"

#include "Hooks/hooks_detours.h"
#include "Overlay/WindowManager.h"
#include "PaletteManager/PaletteManager.h"

#include <Windows.h>

HMODULE hOriginalDinput;

typedef HRESULT(WINAPI *DirectInput8Create_t)(HINSTANCE hinstHandle, DWORD version, const IID& r_iid, LPVOID* outWrapper, LPUNKNOWN pUnk);
DirectInput8Create_t orig_DirectInput8Create;

// Exported function
HRESULT WINAPI DirectInput8Create(HINSTANCE hinstHandle, DWORD version, const IID& r_iid, LPVOID* outWrapper, LPUNKNOWN pUnk)
{
	LOG(1, "DirectInput8Create\n");
	return orig_DirectInput8Create(hinstHandle, version, r_iid, outWrapper, pUnk);
}

void CreateCustomDirectories()
{
	LOG(2, "CreateCustomDirectories\n");

	CreateDirectory(L"BBTAG_IM", NULL);
}

void BBTAG_IM_Shutdown()
{
	WindowManager::GetInstance().Shutdown();
	CleanupInterfaces();
	closeLogger();
}

bool LoadOriginalDinputDll()
{
	if (Settings::settingsIni.dinputDllWrapper.find("none") == 0 || Settings::settingsIni.dinputDllWrapper == "")
	{
		char dllPath[MAX_PATH];
		GetSystemDirectoryA(dllPath, MAX_PATH);
		strcat_s(dllPath, "\\dinput8.dll");

		hOriginalDinput = LoadLibraryA(dllPath);
	}
	else
	{
		LOG(2, "Loading dinput wrapper: %s\n", Settings::settingsIni.dinputDllWrapper.c_str());
		hOriginalDinput = LoadLibraryA(Settings::settingsIni.dinputDllWrapper.c_str());
	}

	if (hOriginalDinput == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	orig_DirectInput8Create = (DirectInput8Create_t)GetProcAddress(hOriginalDinput, "DirectInput8Create");
	if (!orig_DirectInput8Create)
	{
		return false;
	}
	LOG(1, "orig_DirectInput8Create: 0x%p\n", orig_DirectInput8Create);

	return true;
}

DWORD WINAPI BBTAG_IM_Start(HMODULE hModule)
{
	openLogger();
	LOG(1, "Starting BBTAG_IM_Start thread\n");
	CreateCustomDirectories();
	SetUnhandledExceptionFilter(UnhandledExFilter);

	if (!Settings::loadSettingsFile())
	{
		ExitProcess(0);
	}
	logSettingsIni();
	Settings::initSavedSettings();

	if (!LoadOriginalDinputDll())
	{
		MessageBoxA(nullptr, "Could not load original dinput8.dll!", "BBTAGIM", MB_OK);
		ExitProcess(0);
	}

	if (!placeHooks_detours())
	{
		MessageBoxA(nullptr, "Failed IAT hook", "BBTAGIM", MB_OK);
		ExitProcess(0);
	}

	g_interfaces.pPaletteManager = new PaletteManager();

	return 0;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hinstDLL);
		CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)BBTAG_IM_Start, hinstDLL, 0, nullptr));
		break;

	case DLL_PROCESS_DETACH:
		BBTAG_IM_Shutdown();
		FreeLibrary(hOriginalDinput);
		break;
	}
	return TRUE;
}