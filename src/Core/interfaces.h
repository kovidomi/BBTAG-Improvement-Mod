#pragma once
#include "D3D9EXWrapper/ID3D9ExWrapper_Device.h"
#include "Game/Player.h"
#include "PaletteManager/PaletteManager.h"
#include "SteamApiWrapper/SteamFriendsWrapper.h"
#include "SteamApiWrapper/SteamMatchmakingWrapper.h"
#include "SteamApiWrapper/SteamNetworkingWrapper.h"
#include "SteamApiWrapper/SteamUserStatsWrapper.h"
#include "SteamApiWrapper/SteamUserWrapper.h"
#include "SteamApiWrapper/SteamUtilsWrapper.h"
#include "SteamApiWrapper/SteamApiHelper.h"
#include <steam_api.h>

struct lookat_t
{
	D3DXVECTOR3 *pEye;
	D3DXVECTOR3 *pAt;
	D3DXVECTOR3 *pUp;
};

struct interfaces_t
{
	SteamFriendsWrapper* pSteamFriendsWrapper;
	SteamMatchmakingWrapper* pSteamMatchmakingWrapper;
	SteamNetworkingWrapper* pSteamNetworkingWrapper;
	SteamUserWrapper* pSteamUserWrapper;
	SteamUserStatsWrapper* pSteamUserStatsWrapper;
	SteamUtilsWrapper* pSteamUtilsWrapper;
	IDirect3DDevice9Ex* pD3D9ExWrapper;
	SteamApiHelper* pSteamApiHelper;
	PaletteManager* pPaletteManager;
	
	Player player1;
	Player player2;
};

struct gameVals_t
{
	// *pIsHUDHidden is a bitfield:
	// 0x00 - hud is visible
	// 0x01 - hud is hidden (intro)
	// 0x02 - hud is hidden (astral)
	// 0x04 - loading icon is shown
	int* pIsHUDHidden;

	int* pGameTimer;

	int* pGameState;
	int* pGameMode;

	bool isFrameFrozen;
	unsigned framesToReach;
	unsigned* pFrameCount;

	lookat_t lookAtVector;
	D3DXMATRIX* viewMatrix;
	D3DXMATRIX* projMatrix;

	int* pEntityList;
	int entityCount;
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

extern interfaces_t g_interfaces;
extern gameProc_t g_gameProc;
extern gameVals_t g_gameVals;
extern temps_t g_tempVals;

void CleanupInterfaces();