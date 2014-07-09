#pragma once

#include "mongo/client/dbclient.h"

#include "MapInfo.h"

class DataSource
{
protected:
	mongo::DBClientConnection conn;

public:
	DataSource(const std::string& host);

	bool checkConnection();
	bool saveMap(const MapInfo& map, bool autoload);
	bool disposeMap(const std::string& name);
	bool loadMap(const std::string& name, MapInfo& dest);
};
