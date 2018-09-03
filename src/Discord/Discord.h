#pragma once
#include <discord_rpc.h>

class Discord
{
private:
	DiscordRichPresence m_presence;
public:
	Discord(const char* appID, const char* steamID = 0);
	~Discord();
	void UpdatePresence();
	void UpdatePresencePartyState(int partyCurSize, int partyMaxSize, const char* groupStateText, ...);
	void UpdatePresenceDetails(const char* detailsText, ...);
	void UpdatePresenceLargeImage(const char* imageName);
	void UpdatePresenceLargeImageText(const char* imageText);
	void UpdatePresenceSmallImage(const char* imageName);
	void UpdatePresenceSmallImageText(const char* imageText);
	void UpdatePresenceSetStartTime();
	void UpdatePresenceClearStartTime();
	void UpdatePresenceSetEndTime(unsigned int secsRemaining);
	void UpdatePresenceClearEndTime();
private:
	void PrintPresence();
};