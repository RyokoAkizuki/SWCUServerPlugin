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
