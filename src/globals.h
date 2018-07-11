#pragma once
#include "SteamApiWrapper\SteamFriendsWrapper.h"
#include "SteamApiWrapper\SteamMatchmakingWrapper.h"
#include "SteamApiWrapper\SteamNetworkingWrapper.h"
#include "SteamApiWrapper\SteamUserStatsWrapper.h"
#include "SteamApiWrapper\SteamUserWrapper.h"
#include "SteamApiWrapper\SteamUtilsWrapper.h"
#include "steamapi_helper.h"
#include "D3D9EXWrapper\ID3D9ExWrapper_Device.h"
#include "PaletteManager\CharPalInfo.h"
#include "PaletteManager\PaletteManager.h"
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

	CharPalInfos* pCharPalInfos;
	PaletteManager* pPaletteManager;
};

struct gameVals_t
{
	int* pIsHUDHidden;
	int* pGameTimer;

	int* pGameState;
	int* pGameMode;

	CharObj **CharObj_P1Char1;
	CharObj **CharObj_P1Char2;
	CharObj **CharObj_P2Char1;
	CharObj **CharObj_P2Char2;

	PlayerMeters *PlayerMetersObj;

	bool isPaletteModePaused;
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

extern g_interfaces_t g_interfaces;
extern gameProc_t g_gameProc;
extern gameVals_t g_gameVals;
extern temps_t g_tempVals;

void CleanupGlobals();