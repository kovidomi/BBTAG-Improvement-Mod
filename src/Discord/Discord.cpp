#include "Discord.h"
#include <discord_register.h>
#include <string>
#include <time.h>
#include <stdarg.h>
#include "../WindowManager/WindowManager.h"
#pragma comment(lib, "discord-rpc.lib")

Discord::Discord(const char * appID, const char * steamID)
{
	m_presence = {};

	DiscordEventHandlers handlers;
	memset(&handlers, 0, sizeof(handlers));
	//handlers.ready = handleDiscordReady;
	//handlers.errored = handleDiscordError;
	//handlers.disconnected = handleDiscordDisconnected;
	//handlers.joinGame = handleDiscordJoinGame;
	//handlers.spectateGame = handleDiscordSpectateGame;
	//handlers.joinRequest = handleDiscordJoinRequest;

	Discord_Initialize(appID, &handlers, 1, steamID);
}

Discord::~Discord()
{
	// Causes the game to hang for some reason
	// Discord_Shutdown();
}

void Discord::UpdatePresence()
{
	char buffer[128];
	DiscordRichPresence m_presence = {};

	m_presence.state = "In a Group";
	// sprintf(buffer, "Ranked | Mode: %d", GameEngine.GetMode());
	sprintf(buffer, "Ranked | Mode: %s", "Training");
	m_presence.details = buffer;
	m_presence.endTimestamp = time(0) + 5 * 60;
	m_presence.largeImageKey = "puffy";
	m_presence.smallImageKey = "puffy";
	//m_presence.partyId = GameEngine.GetPartyId();
	//m_presence.partySize = 1;
	//m_presence.partyMax = 6;
	//m_presence.matchSecret = "4b2fdce12f639de8bfa7e3591b71a0d679d7c93f";
	//m_presence.spectateSecret = "e7eb30d2ee025ed05c71ea495f770b76454ee4e0";
	m_presence.instance = 1;
	Discord_UpdatePresence(&m_presence);
}

void Discord::UpdatePresencePartyState(int partyCurSize, int partyMaxSize, const char* partyStateText, ...)
{
	char buf[128];

	va_list	va_alist;
	va_start(va_alist, partyStateText);
	_vsnprintf_s(buf, sizeof(buf), partyStateText, va_alist);
	va_end(va_alist);

	m_presence.state = buf;
	m_presence.partySize = partyCurSize;
	m_presence.partyMax = partyMaxSize;

	Discord_UpdatePresence(&m_presence);
}

void Discord::UpdatePresenceDetails(const char* detailsText, ...)
{
	char buf[128];

	va_list	va_alist;
	va_start(va_alist, detailsText);
	_vsnprintf_s(buf, sizeof(buf), detailsText, va_alist);
	va_end(va_alist);

	m_presence.details = buf;

	Discord_UpdatePresence(&m_presence);
}

void Discord::UpdatePresenceLargeImage(const char* imageName) 
{
	m_presence.largeImageKey = imageName;
	Discord_UpdatePresence(&m_presence);
}

void Discord::UpdatePresenceLargeImageText(const char * imageText)
{
	m_presence.largeImageText = imageText;
	Discord_UpdatePresence(&m_presence);
}

void Discord::UpdatePresenceSmallImage(const char* imageName)
{
	m_presence.smallImageKey = imageName;
	Discord_UpdatePresence(&m_presence);
}

void Discord::UpdatePresenceSmallImageText(const char * imageText)
{
	m_presence.smallImageText = imageText;
	Discord_UpdatePresence(&m_presence);
}

void Discord::UpdatePresenceSetStartTime()
{
	m_presence.startTimestamp = time(0);
	Discord_UpdatePresence(&m_presence);
}

void Discord::UpdatePresenceClearStartTime()
{
	m_presence.startTimestamp = 0;
	Discord_UpdatePresence(&m_presence);
}

void Discord::UpdatePresenceSetEndTime(unsigned int secsRemaining)
{
	m_presence.endTimestamp = time(0) + secsRemaining;
	Discord_UpdatePresence(&m_presence);
}

void Discord::UpdatePresenceClearEndTime()
{
	m_presence.endTimestamp = 0;
	Discord_UpdatePresence(&m_presence);
}

void Discord::PrintPresence()
{
	WindowManager::AddLog("[debug]\nDetails: %s \nLarge Image: %s \nSmall Image: %s \nStart Time: %d \nEnd Time: %d\n",
		m_presence.details, m_presence.largeImageKey, m_presence.smallImageKey, 
		m_presence.startTimestamp, m_presence.endTimestamp);

	WindowManager::AddLogSeparator();
}
