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
#include <sampgdk/streamer.h>

#include "GameServer.h"
#include "AccountDialogs.h"
#include "WeaponShop.h"
#include "StringUtility.h"
#include "AdminCommands.h"
#include "GeneralPanel.h"
#include "SAMPFunctionUtility.h"

using sampgdk::logprintf;

#define SUPPORTS_PROCESS_TICK 0x20000

void runServer(); // defined in RPCServer.cpp

std::thread rpcServer;

void SAMPGDK_CALL PrintTickCountTimer(int timerid, void *params)
{
	logprintf("Tick count: %d", GetTickCount());
}

PLUGIN_EXPORT bool PLUGIN_CALL OnGameModeInit()
{
	rpcServer = std::thread(&runServer);
	GameServer::getInstance().mapmanager.loadAutoLoadMaps();
	return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerConnect(int playerid)
{
	if (GameServer::getInstance().datasource.hasBanRecord(getPlayerNameFixed(playerid), getPlayerIP(playerid), getGPCI(playerid)))
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

	acc->isRegistered() ? showLoginDialog(acc) : showRegisterDialog(acc);
	return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerDisconnect(int playerid, int reason)
{
	GameServer::getInstance().accountmanager.exitServer(playerid);
	return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerSpawn(int playerid)
{
	GameServer::getInstance().housemanager.initAll();
	return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerCommandText(int playerid, const char *cmdtext)
{
	std::shared_ptr<Account> acc = GameServer::getInstance().accountmanager.findAccount(playerid);

	return false;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnDialogResponse(int playerid, int dialogid, int response, int listitem, const char * inputtext)
{
	return GameServer::getInstance().dialogmanager._callback(playerid, dialogid, response, listitem, inputtext);
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerClickPlayer(int playerid, int clickedplayerid, int source)
{
	if (!IsPlayerConnected(clickedplayerid))
	{
		SendClientMessage(playerid, 0xFFFFFFFF, "[Account] 对方不在线.");
		return false;
	}

	std::shared_ptr<Account> acc = GameServer::getInstance().accountmanager.findAccount(playerid);

	if (!acc->isLoggedIn())
	{
		SendClientMessage(playerid, 0xFFFFFFFF, "[Account] 你没有登录.");
		return false;
	}

	if (playerid == clickedplayerid)
	{
		showGeneralPanel(acc);
	}
	else
	{
		showPlayerAdminDialog(acc, GameServer::getInstance().accountmanager.findAccount(clickedplayerid));
	}

	return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerText(int playerid, const char * text)
{
	if (GameServer::getInstance().accountmanager.findAccount(playerid)->isMuted())
	{
		SendClientMessage(playerid, 0xFFFFFFFF, "[Account] 你已被禁言.");
		return false;
	}
	return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerUpdate(int playerid)
{
	auto acc = GameServer::getInstance().accountmanager.findAccount(playerid);
	if (acc->isFreezed())
	{
		return false;
	}
	// record this frame data
	auto& info = acc->_getAccountInfo();
	GetPlayerPos(playerid, &info.x, &info.y, &info.z);
	return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerEnterDynamicArea(int playerid, int areaid)
{
	GameServer::getInstance().areamanager._callbackEnter(GameServer::getInstance().accountmanager.findAccount(playerid), areaid);
	SendClientMessage(playerid, 0xFFFFFFFF, "Entered");
	return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerLeaveDynamicArea(int playerid, int areaid)
{
	GameServer::getInstance().areamanager._callbackExit(GameServer::getInstance().accountmanager.findAccount(playerid), areaid);
	SendClientMessage(playerid, 0xFFFFFFFF, "Exited");
	return true;
}

PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports()
{
	return sampgdk::Supports() | SUPPORTS_PROCESS_TICK;
}

PLUGIN_EXPORT bool PLUGIN_CALL Load(void **ppData)
{
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
