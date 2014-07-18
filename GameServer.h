#pragma once

#include "DataSource.h"
#include "AccountManager.h"
#include "MapManager.h"
#include "DialogManager.h"
#include "AreaManager.h"
#include "HouseManager.h"

class GameServer
{
public:
	DataSource datasource;
	AccountManager accountmanager;
	MapManager mapmanager;
	DialogManager dialogmanager;
	AreaManager areamanager;
	HouseManager housemanager;

	static GameServer& getInstance();

protected:
	GameServer(const std::string& dbhost);
};
