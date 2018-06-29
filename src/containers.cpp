#include "../include/containers.h"
#include "../include/utils.h"

g_interfaces_t Containers::g_interfaces = {};
temps_t Containers::tempVals = {};
gameProc_t Containers::gameProc = {};
gameVals_t Containers::gameVals = {};

void Containers::Cleanup()
{
	SAFE_DELETE(g_interfaces.pD3D9ExWrapper);
	SAFE_DELETE(g_interfaces.pSteamFriendsWrapper);
	SAFE_DELETE(g_interfaces.pSteamMatchmakingWrapper);
	SAFE_DELETE(g_interfaces.pSteamNetworkingWrapper);
	SAFE_DELETE(g_interfaces.pSteamUserStatsWrapper);
	SAFE_DELETE(g_interfaces.pSteamUserWrapper);
	SAFE_DELETE(g_interfaces.pSteamUtilsWrapper);
	SAFE_DELETE(g_interfaces.pSteamApiHelper);
}
