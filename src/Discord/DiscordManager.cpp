#include "DiscordManager.h"
#include <discord_register.h>
#include <discord_rpc.h>
#include <string>
#include <time.h>
#pragma comment(lib, "discord-rpc.lib")

void DiscordManager::InitDiscord()
{
	DiscordEventHandlers handlers;
	memset(&handlers, 0, sizeof(handlers));
	//handlers.ready = handleDiscordReady;
	//handlers.errored = handleDiscordError;
	//handlers.disconnected = handleDiscordDisconnected;
	//handlers.joinGame = handleDiscordJoinGame;
	//handlers.spectateGame = handleDiscordSpectateGame;
	//handlers.joinRequest = handleDiscordJoinRequest;

	// Discord_Initialize(const char* applicationId, DiscordEventHandlers* handlers, int autoRegister, const char* optionalSteamId)
	Discord_Initialize("485863441610047488", &handlers, 1, "702890");
}

void DiscordManager::UpdatePresence()
{
	char buffer[256];
	DiscordRichPresence discordPresence;
	memset(&discordPresence, 0, sizeof(discordPresence));
	discordPresence.state = "In a Group";
	// sprintf(buffer, "Ranked | Mode: %d", GameEngine.GetMode());
	sprintf(buffer, "Ranked | Mode: %s", "Training");
	discordPresence.details = buffer;
	discordPresence.endTimestamp = time(0) + 5 * 60;
	discordPresence.largeImageKey = "puffy";
	discordPresence.smallImageKey = "puffy";
	//discordPresence.partyId = GameEngine.GetPartyId();
	//discordPresence.partySize = 1;
	//discordPresence.partyMax = 6;
	//discordPresence.matchSecret = "4b2fdce12f639de8bfa7e3591b71a0d679d7c93f";
	//discordPresence.spectateSecret = "e7eb30d2ee025ed05c71ea495f770b76454ee4e0";
	discordPresence.instance = 1;
	Discord_UpdatePresence(&discordPresence);
}

void DiscordManager::ShutdownDiscord()
{
	Discord_Shutdown();
}