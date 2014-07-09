#include <iostream>
#include <string>

#include "MapFunctions.h"
#include "MapManager.h"

bool MapManager::save(const MapInfo& map, bool autoload)
{
	return datasource.saveMap(map, autoload);
}

bool MapManager::dispose(const std::string& name)
{
	return datasource.disposeMap(name);
}

bool MapManager::load(const std::string& name)
{
	MapInfo dest;
	bool loaded = isLoaded(name);

	if (loaded)
	{
		std::cout << "[MapManager] Map " << name << " is already loaded. Will try to reload it from datasource.\n";
	}
	if (datasource.loadMap(name, dest))
	{
		if (loaded)
		{
			unload(name);
		}

		auto iter = loadedmap.insert(std::make_pair(name, std::make_pair(std::vector<int>(), std::vector<int>())));
		auto objs = iter.first->second.first;
		auto vehs = iter.first->second.second;

		for (auto i : dest.objects)
		{
			objs.push_back(createObject(i));
		}
		for (auto i : dest.vehicles)
		{
			vehs.push_back(createVehicle(i));
		}

		std::cout << "[MapManager] Map " << name << " loaded. Map info: " << dest << "\n";
	}
	else
	{
		std::cout << "[MapManager] Failed to load map " << name << ". Did nothing.\n";
		return false;
	}

	return true;
}

bool MapManager::unload(const std::string& name)
{
	auto iter = loadedmap.find(name);
	if (iter == loadedmap.end())
	{
		std::cout << "[MapManager] Map " << name << " is not loaded. Unload request ignored.\n";
		return false;
	}
	auto objs = iter->second.first;
	auto vehs = iter->second.second;
	for (auto i : objs)
	{
		destroyObject(i);
	}
	for (auto i : vehs)
	{
		destroyVehicle(i);
	}
	std::cout << "[MapManager] Map " << name << " unloaded.\n";
	return true;
};

bool MapManager::isLoaded(const std::string& name)
{
	return loadedmap.count(name);
}
