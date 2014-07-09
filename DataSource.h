#pragma once

#include <memory>

#include "MapInfo.h"

namespace mongo
{
	class DBClientConnection;
}

class DataSource
{
protected:
	std::unique_ptr<mongo::DBClientConnection> conn;

public:
	DataSource(const std::string& host);
	virtual ~DataSource();

	bool saveMap(const MapInfo& map, bool autoload);
	bool disposeMap(const std::string& name);
	bool loadMap(const std::string& name, MapInfo& dest);
	// <name, autoload>
	void getMapList(std::vector<std::pair<std::string, bool>>& list, bool onlyAutoLoad = false);
	bool getMapInfo(const std::string& name, MapInfo& dest);
	bool setMapAutoLoad(const std::string& name, bool autoload);
};
