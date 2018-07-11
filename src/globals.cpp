#include "globals.h"
#include "utils.h"

g_interfaces_t g_interfaces = {};
gameProc_t g_gameProc = {};
gameVals_t g_gameVals = {};
temps_t g_tempVals = {};

void CleanupGlobals()
{
	SAFE_DELETE(g_interfaces.pD3D9ExWrapper);
	SAFE_DELETE(g_interfaces.pSteamFriendsWrapper);
	SAFE_DELETE(g_interfaces.pSteamMatchmakingWrapper);
	SAFE_DELETE(g_interfaces.pSteamNetworkingWrapper);
	SAFE_DELETE(g_interfaces.pSteamUserStatsWrapper);
	SAFE_DELETE(g_interfaces.pSteamUserWrapper);
	SAFE_DELETE(g_interfaces.pSteamUtilsWrapper);
	SAFE_DELETE(g_interfaces.pSteamApiHelper);

	SAFE_DELETE(g_interfaces.pCharPalInfos);
	SAFE_DELETE(g_interfaces.pPaletteManager);
}
