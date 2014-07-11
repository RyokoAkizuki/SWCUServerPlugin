#include <stdio.h>
#include <string.h>
#include <thread>

#include <plugincommon.h>

#include <sampgdk/a_players.h>
#include <sampgdk/a_samp.h>
#include <sampgdk/a_vehicles.h>
#include <sampgdk/core.h>
#include <sampgdk/sdk.h>

#include "GameServer.h"

using sampgdk::logprintf;

#define SUPPORTS_PROCESS_TICK 0x20000

void runServer(); // defined in RPCServer.cpp

std::thread rpcServer;

void SAMPGDK_CALL PrintTickCountTimer(int timerid, void *params)
{
	logprintf("Tick count: %d", GetTickCount());
}

void showLoginDialog(Account& player)
{
	GameServer::getInstance().dialogmanager.displayInputDialog(player, "µÇÂ¼", "ÇëÊäÈëÃÜÂë",
		"µÇÂ¼", "ÍË³ö·þÎñÆ÷", false,
		[&player](const std::string& pw) {
			if (!player.auth(pw))
			{
				showLoginDialog(player);
			}
		},
		[&player](const std::string& pw) { Kick(player.getInGameID()); });
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerConnect(int playerid)
{
	Account& acc = GameServer::getInstance().accountmanager.enterServer(playerid);
	if (!acc.isRegistered())
	{
		GameServer::getInstance().dialogmanager.displayInputDialog(acc, "×¢²á", "ÇëÊäÈëÃÜÂë",
			"×¢²á", "ÍË³ö·þÎñÆ÷", false,
			[&acc](const std::string& pw) { acc.create(pw); },
			[playerid](const std::string& pw) { Kick(playerid); });
	}
	else
	{
		showLoginDialog(acc);
	}
	return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerDisconnect(int playerid, int reason)
{
	GameServer::getInstance().accountmanager.exitServer(playerid);
	return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerCommandText(int playerid, const char *cmdtext)
{
	if (strcmp(cmdtext, "/car") == 0) {
		float pos[3];
		GetPlayerPos(playerid, &pos[0], &pos[1], &pos[2]);
		CreateVehicle(411, pos[0], pos[1], pos[2], 0.0, 1, 1, 120);
		return true;
	}
	return false;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnDialogResponse(int playerid, int dialogid, int response, int listitem, const char * inputtext)
{
	GameServer::getInstance().dialogmanager._callback(playerid, dialogid, response, listitem, inputtext);
	return true;
}

PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports()
{
	return sampgdk::Supports() | SUPPORTS_PROCESS_TICK;
}

PLUGIN_EXPORT bool PLUGIN_CALL Load(void **ppData)
{
	rpcServer = std::thread(&runServer);
	return sampgdk::Load(ppData);
}

PLUGIN_EXPORT void PLUGIN_CALL Unload()
{
	sampgdk::Unload();
}

PLUGIN_EXPORT void PLUGIN_CALL ProcessTick()
{
	sampgdk::ProcessTick();
}
