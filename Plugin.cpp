#include <stdio.h>
#include <string.h>
#include <thread>
#include <sstream>

#include <plugincommon.h>

#include <sampgdk/a_players.h>
#include <sampgdk/a_samp.h>
#include <sampgdk/a_vehicles.h>
#include <sampgdk/core.h>
#include <sampgdk/sdk.h>

#include "GameServer.h"
#include "DialogDefinitions.h"
#include "WeaponShop.h"

using sampgdk::logprintf;

#define SUPPORTS_PROCESS_TICK 0x20000

void runServer(); // defined in RPCServer.cpp

std::thread rpcServer;

void SAMPGDK_CALL PrintTickCountTimer(int timerid, void *params)
{
	logprintf("Tick count: %d", GetTickCount());
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerConnect(int playerid)
{
	Account& acc = GameServer::getInstance().accountmanager.enterServer(playerid);
	acc.isRegistered() ? showLoginDialog(acc) : showRegisterDialog(acc);
	return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerDisconnect(int playerid, int reason)
{
	GameServer::getInstance().accountmanager.exitServer(playerid);
	return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerCommandText(int playerid, const char *cmdtext)
{
	Account& acc = GameServer::getInstance().accountmanager.findAccount(playerid);

	if (strcmp(cmdtext, "/car") == 0) {
		float pos[3];
		GetPlayerPos(playerid, &pos[0], &pos[1], &pos[2]);
		CreateVehicle(411, pos[0], pos[1], pos[2], 0.0, 1, 1, 120);
		return true;
	}
	if (strcmp(cmdtext, "/weapon") == 0) {
		showWeaponShopDialog(acc);
		return true;
	}
	return false;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnDialogResponse(int playerid, int dialogid, int response, int listitem, const char * inputtext)
{
	GameServer::getInstance().dialogmanager._callback(playerid, dialogid, response, listitem, inputtext);
	return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerClickPlayer(int playerid, int clickedplayerid, int source)
{
	Account& acc = GameServer::getInstance().accountmanager.findAccount(playerid);

	if (!acc.isLoggedIn())
	{
		SendClientMessage(playerid, 0xFFFFFFFF, "[Account] ÄãÃ»ÓÐµÇÂ¼.");
		return false;
	}

	if (playerid == clickedplayerid)
	{
		showAccountOptionsDialog(acc);
	}

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
