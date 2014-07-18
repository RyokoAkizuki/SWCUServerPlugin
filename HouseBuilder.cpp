/*
 * Copyright 2014 Yukino Hayakawa<tennencoll@gmail.com>
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
