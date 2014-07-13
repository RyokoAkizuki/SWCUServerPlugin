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
#include "AccountDialogs.h"
#include "WeaponShop.h"
#include "StringUtility.h"
#include "AdminCommands.h"

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
	char ip[16];
	GetPlayerIp(playerid, ip, 16);
	char gpcidest[25];
	gpci(playerid, gpcidest, 25);
	char logname[24];
	GetPlayerName(playerid, logname, 24);

	if (GameServer::getInstance().datasource.hasBanRecord(logname, ip, gpcidest))
	{
		SendClientMessage(playerid, 0xFFFFFFFF, "[Account] 你的客户端已被封禁.");
		Kick(playerid);
		return false;
	}

	std::shared_ptr<Account> acc = GameServer::getInstance().accountmanager.enterServer(playerid);

	if (acc->isDisabled())
	{
		SendClientMessage(playerid, 0xFFFFFFFF, "[Account] 你的账号已被封禁.");
		Kick(playerid);
		return false;
	}

	SendClientMessageToAll(0xFFFFFFFF, STR(acc->getLogName() << UID(acc) << "进入服务器.").c_str());

	acc->isRegistered() ? showLoginDialog(acc) : showRegisterDialog(acc);
	return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerDisconnect(int playerid, int reason)
{
	GameServer::getInstance().accountmanager.exitServer(playerid);
	return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerCommandText(int playerid, const char *cmdtext)
{
	std::shared_ptr<Account> acc = GameServer::getInstance().accountmanager.findAccount(playerid);

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
	std::shared_ptr<Account> acc = GameServer::getInstance().accountmanager.findAccount(playerid);

	if (!acc->isLoggedIn())
	{
		SendClientMessage(playerid, 0xFFFFFFFF, "[Account] 你没有登录.");
		return false;
	}

	showAdminDialog(acc, GameServer::getInstance().accountmanager.findAccount(clickedplayerid));

	return true;

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
