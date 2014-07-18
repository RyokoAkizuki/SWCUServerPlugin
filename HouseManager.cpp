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

#include "GameServer.h"

#include "HouseManager.h"

// NOTICE: may cause inconsistency of data if player is holding another instance
void HouseManager::reloadAll()
{
	houses.clear();
	std::vector<HouseInfo> dest;
	GameServer::getInstance().datasource.loadHouses(dest);
	for (auto i : dest)
	{
		houses.insert(std::make_pair(i.houseid, std::make_shared<House>(i)));
	}
}

std::shared_ptr<House> HouseManager::findHouse(const std::string& houseid)
{ 
	auto iter = houses.find(houseid);
	if (iter == houses.end())
	{
		std::cout << "[HouseManager] Warning: invalid house id.\n";
		return std::make_shared<House>(HouseInfo());
	}
	return iter->second;
}

std::shared_ptr<House> HouseManager::addHouse(HouseInfo& info)
{
	if (!GameServer::getInstance().datasource.createHouse(info))
	{
		std::cout << "[HouseManager] Warning: createHouse failed.\n";
		return std::make_shared<House>(HouseInfo());
	}
	else
	{
		auto r = houses.insert(std::make_pair(info.houseid, std::make_shared<House>(info)));
		return r.first->second;
	}
}

void HouseManager::initAll()
{
	for (auto i : houses)
	{
		i.second->init();
	}
}
