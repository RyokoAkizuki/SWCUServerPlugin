#include <sampgdk/a_samp.h>
#include <sampgdk/a_players.h>
#include <sampgdk/streamer.h>
#include <sampgdk/a_objects.h>

#include "GameServer.h"
#include "House.h"
#include "StringUtility.h"
#include "HouseDialogs.h"

House::House(HouseInfo& info) : info(info)
{
	areaid = CreateDynamicCube(info.lx, info.ly, info.lz, info.mx, info.my, info.mz, -1, -1, -1);
	GameServer::getInstance().areamanager.registerArea(areaid,
	[info](const std::shared_ptr<Account>& player) {
		if (player->getUserID() == info.ownerid)
		{
			SendClientMessage(player->getInGameID(), 0xFFFFFFFF, STR("[House] 欢迎来到你的房产 " << info.name).c_str());
		}
		else
		{
			SendClientMessage(player->getInGameID(), 0xFFFFFFFF, STR("[House] 你正在进入限制区域 " << info.name << " 请输入密码").c_str());
			showHouseAuthDialog(player, GameServer::getInstance().housemanager.findHouse(info.houseid));
		}
	});
	zoneid = GangZoneCreate(info.lx, info.ly, info.mx, info.my);
	signid = CreateDynamicObject(3337, info.ex, info.ey, info.ez, 0.0f, 0.0f, info.rotation, -1, -1, -1, 200.0f, 200.0f);
	SetDynamicObjectMaterialText(signid, 1, info.name.c_str(), OBJECT_MATERIAL_SIZE_256x128, "Arial", 32, 1, 0xFFFFFFFF, 0xFF003366, 1);
	init();
}

House::~House()
{
	GameServer::getInstance().areamanager.destroyArea(areaid);
	GangZoneDestroy(zoneid);
	DestroyDynamicObject(signid);
}

void House::init()
{
	GangZoneShowForAll(zoneid, 0x6699CC88);
}

bool House::setEntrance(float x, float y, float z, float rotation)
{
	bool ret = GameServer::getInstance().datasource.setHouseEntrance(info, x, y, z, rotation);
	if (ret)
	{
		SetDynamicObjectPos(signid, x, y, z);
		SetDynamicObjectRot(signid, 0.0, 0.0, rotation);
	}
	return ret;
}

bool House::setTeleportPos(float x, float y, float z)
{
	return GameServer::getInstance().datasource.setHouseTeleportPos(info, x, y, z);
}

bool House::setName(const std::string& name)
{
	bool ret = GameServer::getInstance().datasource.setHouseName(info, name);
	if (ret)
	{
		SetDynamicObjectMaterialText(signid, 1, info.name.c_str(), OBJECT_MATERIAL_SIZE_256x128, "Arial", 32, 1, 0xFFFFFFFF, 0xFF003366, 1);
	}
	return ret;
}

bool House::setPassword(const std::string& password)
{
	return GameServer::getInstance().datasource.setHousePassword(info, password);
}

std::string House::getName()
{
	return info.name;
}

bool House::auth(const std::string& password)
{
	return info.password == password;
}

HouseInfo& House::_getInfo()
{
	return info;
}
