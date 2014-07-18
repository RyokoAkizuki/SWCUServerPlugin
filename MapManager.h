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

#pragma once

#include <vector>
#include <map>

#include "MapInfo.h"
#include "DataSource.h"

class MapManager
{
protected:
	// <name, <obj uid, veh uid>>
	std::map<std::string, std::pair<std::vector<int>, std::vector<int>>> loadedmap;
	DataSource& datasource;

public:
	MapManager(DataSource& datasrc) : datasource(datasrc) {}

	DataSource& getDataSource() { return datasource;  }

	bool save(const MapInfo& map, bool autoload);
	bool dispose(const std::string& name);

	bool load(const std::string& name);
	bool unload(const std::string& name);
	bool isLoaded(const std::string& name);

	void loadAutoLoadMaps();
};
