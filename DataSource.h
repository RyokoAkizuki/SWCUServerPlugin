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

	bool checkConnection();
	bool saveMap(const MapInfo& map, bool autoload);
	bool disposeMap(const std::string& name);
	bool loadMap(const std::string& name, MapInfo& dest);
	void getMapList(std::vector<std::string>& list, bool onlyAutoLoad = false);
};
