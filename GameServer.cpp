#include "GameServer.h"

GameServer& GameServer::getInstance()
{
	static GameServer ins("localhost");
	return ins;
}

GameServer::GameServer(const std::string& dbhost)
: datasource(dbhost), accountmanager(datasource), mapmanager(datasource), dialogmanager(accountmanager)
{
	housemanager.reloadAll();
}
