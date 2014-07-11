#pragma once

#include "DataSource.h"
#include "AccountManager.h"
#include "MapManager.h"
#include "DialogManager.h"

class GameServer
{
public:
	DataSource datasource;
	AccountManager accountmanager;
	MapManager mapmanager;
	DialogManager dialogmanager;
	
	static GameServer& getInstance();

protected:
	GameServer(const std::string& dbhost);
};
