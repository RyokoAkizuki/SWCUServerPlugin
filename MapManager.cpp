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
		std::vector<int>& objs = iter.first->second.first;
		std::vector<int>& vehs = iter.first->second.second;

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
	std::vector<int>& objs = iter->second.first;
	std::vector<int>& vehs = iter->second.second;
	for (auto i : objs)
	{
		destroyObject(i);
	}
	for (auto i : vehs)
	{
		destroyVehicle(i);
	}
	loadedmap.erase(iter);
	std::cout << "[MapManager] Map " << name << " unloaded.\n";
	return true;
};

bool MapManager::isLoaded(const std::string& name)
{
	return loadedmap.count(name) != 0;
}

void MapManager::loadAutoLoadMaps()
{
	std::cout << "[MapManager] Autoloading...\n";
	std::vector<std::pair<std::string, bool>> list;
	datasource.getMapList(list, true);
	for (auto i : list)
	{
		load(i.first);
	}
	std::cout << "[MapManager] Autoload: Loaded " << list.size() << " maps.\n";
}
