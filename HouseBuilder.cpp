#include <algorithm>

#include "HouseBuilder.h"
#include "GameServer.h"

HouseBuilder::HouseBuilder() : updating(false)
{

}

void HouseBuilder::setName(const std::string& name)
{
	info.name = name;
}

void HouseBuilder::setEntrance(float x, float y, float z, float rotation)
{
	info.ex = x; info.ey = y; info.ez = z; info.rotation = rotation;
	info.tx = x; info.ty = y; info.tz = z;
}

void HouseBuilder::setOwner(const std::string& playerid)
{
	info.ownerid = playerid;
}

void HouseBuilder::startUpdateArea()
{
	updating = true;
}

void HouseBuilder::stopUpdateArea()
{
	updating = false;
}

bool HouseBuilder::isUpdatingArea()
{
	return updating;
}

void HouseBuilder::updateArea(float x, float y, float z)
{
	if (info.lx == 0) info.lx = x;
	if (info.ly == 0) info.ly = y;
	if (info.lz == 0) info.lz = z;
	if (info.mx == 0) info.mx = x;
	if (info.my == 0) info.my = y;
	if (info.mz == 0) info.mz = z;
	info.lx = std::min(info.lx, x);
	info.ly = std::min(info.ly, y);
	info.lz = std::min(info.lz, z);
	info.mx = std::max(info.mx, x);
	info.my = std::max(info.my, y);
	info.mz = std::max(info.mz, z);
}

void HouseBuilder::save()
{
	info.lx -= 3.0; info.ly -= 3.0; info.lz -= 5.0;
	info.mx += 3.0; info.my += 3.0; info.mz += 30.0;
	GameServer::getInstance().housemanager.addHouse(info);
}
