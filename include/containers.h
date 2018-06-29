#pragma once
#include "SteamApiWrapper\SteamFriendsWrapper.h"
#include "SteamApiWrapper\SteamMatchmakingWrapper.h"
#include "SteamApiWrapper\SteamNetworkingWrapper.h"
#include "SteamApiWrapper\SteamUserStatsWrapper.h"
#include "SteamApiWrapper\SteamUserWrapper.h"
#include "SteamApiWrapper\SteamUtilsWrapper.h"
#include "steamapi_helper.h"
#include "D3D9EXWrapper\ID3D9ExWrapper_Device.h"
#include "charobj.h"
#include "playermeters.h"
#include <steam_api.h>

struct g_interfaces_t
{
	SteamFriendsWrapper* pSteamFriendsWrapper;
	SteamMatchmakingWrapper* pSteamMatchmakingWrapper;
	SteamNetworkingWrapper* pSteamNetworkingWrapper;
	SteamUserWrapper* pSteamUserWrapper;
	SteamUserStatsWrapper* pSteamUserStatsWrapper;
	SteamUtilsWrapper* pSteamUtilsWrapper;
	IDirect3DDevice9Ex* pD3D9ExWrapper;
	SteamApiHelper* pSteamApiHelper;
};

struct gameVals_t
{
	int* pIsHUDHidden;
	int* pGameTimer;

	int* pGameState;

	CharObj **CharObj_P1Char1;
	CharObj **CharObj_P1Char2;
	CharObj **CharObj_P2Char1;
	CharObj **CharObj_P2Char2;

	PlayerMeters *PlayerMetersObj;

	int* PalIndex_P1Char1;
	int* PalIndex_P1Char2;
	int* PalIndex_P2Char1;
	int* PalIndex_P2Char2;
};

struct gameProc_t
{
	HWND hWndGameWindow;
};

//temporarily holds the addresses until the wrappers are created 
struct temps_t
{
	ISteamFriends** ppSteamFriends;
	ISteamMatchmaking** ppSteamMatchmaking;
	ISteamNetworking** ppSteamNetworking;
	ISteamUser** ppSteamUser;
	ISteamUserStats** ppSteamUserStats;
	ISteamUtils** ppSteamUtils;
};

class Containers
{
public:
	static g_interfaces_t g_interfaces;
	static gameProc_t gameProc;
	static gameVals_t gameVals;
	static temps_t tempVals;
	static void Cleanup();
};