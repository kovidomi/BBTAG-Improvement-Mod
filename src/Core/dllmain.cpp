#include "crashdump.h"
#include "interfaces.h"
#include "logger.h"
#include "Settings.h"

#include "Hooks/hooks_detours.h"
#include "PaletteManager/PaletteManager.h"
#include "WindowManager/WindowManager.h"

#include <Windows.h>

typedef HRESULT(WINAPI *DirectInput8Create_t)(HINSTANCE inst_handle, DWORD version, const IID& r_iid, LPVOID* out_wrapper, LPUNKNOWN p_unk);
DirectInput8Create_t orig_DirectInput8Create;

// Exported function
HRESULT WINAPI DirectInput8Create(HINSTANCE inst_handle, DWORD version, const IID& r_iid, LPVOID* out_wrapper, LPUNKNOWN p_unk)
{
	LOG(1, "DirectInput8Create\n");
	return orig_DirectInput8Create(inst_handle, version, r_iid, out_wrapper, p_unk);
}

void CreateCustomDirectories()
{
	LOG(2, "CreateCustomDirectories\n");

	CreateDirectory(L"BBTAG_IM", NULL);
}

void BBTAG_IM_Shutdown()
{
	WindowManager::Shutdown();
	CleanupInterfaces();
	closeLogger();
}

bool LoadOriginalDinputDll()
{
	char dllPath[MAX_PATH];
	GetSystemDirectoryA(dllPath, MAX_PATH);
	strcat_s(dllPath, "\\dinput8.dll");

	HMODULE hMod = LoadLibraryA(dllPath);
	if (hMod == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	orig_DirectInput8Create = (DirectInput8Create_t)GetProcAddress(hMod, "DirectInput8Create");
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

	if (!LoadOriginalDinputDll())
	{
		MessageBoxA(NULL, "Could not load original dinput8.dll!", "BBTAGIM", MB_OK);
		ExitProcess(0);
	}

	if (Settings::loadSettingsFile())
	{
		logSettingsIni();
		Settings::initSavedSettings();

		if (!placeHooks_detours())
		{
			MessageBoxA(NULL, "Failed IAT hook", "BBTAGIM", MB_OK);
			ExitProcess(0);
		}

		g_interfaces.pPaletteManager = new PaletteManager();
	}

	return 0;
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule);
		CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)BBTAG_IM_Start, hModule, 0, nullptr));
		break;

	case DLL_PROCESS_DETACH:
		BBTAG_IM_Shutdown();
		break;
	}
	return TRUE;
}