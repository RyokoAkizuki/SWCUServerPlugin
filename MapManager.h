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

	bool save(const MapInfo& map, bool autoload);
	bool dispose(const std::string& name);

	bool load(const std::string& name);
	bool unload(const std::string& name);
	bool isLoaded(const std::string& name);

	void loadAutoLoadMaps();
};
