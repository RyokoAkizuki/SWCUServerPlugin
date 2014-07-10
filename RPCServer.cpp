#include <XmlRpc.h>

#include <iostream>
#include <sstream>
#include <algorithm>
#include <memory>
#include <thread>
#include <chrono>

#include "DataSource.h"
#include "MapCodeParser.h"
#include "MapManager.h"

using namespace XmlRpc;

XmlRpcServer s;

std::unique_ptr<DataSource> g_datasource;
std::unique_ptr<MapManager> g_mapmanager;

class Hello : public XmlRpcServerMethod
{
public:
	Hello(XmlRpcServer* s) : XmlRpcServerMethod("hello", s) {}

	void execute(XmlRpcValue& params, XmlRpcValue& result)
	{
		result = "Hello.";
	}
} hello(&s);   

class MapSave : public XmlRpcServerMethod
{
public:
	MapSave(XmlRpcServer* s) : XmlRpcServerMethod("mapSave", s) {}

	void execute(XmlRpcValue& params, XmlRpcValue& result)
	{
		MapInfo dest;
		dest.name = params[0];

		parseMapCode(params[1], dest);
		
		std::stringstream r;
		r << g_mapmanager->save(dest, params[2]);
		result = r.str();
	}
} mapSave(&s);

class MapDispose : public XmlRpcServerMethod
{
public:
	MapDispose(XmlRpcServer* s) : XmlRpcServerMethod("mapDispose", s) {}

	void execute(XmlRpcValue& params, XmlRpcValue& result)
	{
		std::stringstream r;
		r << g_mapmanager->dispose(params[0]);
		result = r.str();
	}
} mapDispose(&s);

class MapLoad : public XmlRpcServerMethod
{
public:
	MapLoad(XmlRpcServer* s) : XmlRpcServerMethod("mapLoad", s) {}

	void execute(XmlRpcValue& params, XmlRpcValue& result)
	{
		std::stringstream r;
		r << g_mapmanager->load(params[0]);
		result = r.str();
	}
} mapLoad(&s);

class MapUnload : public XmlRpcServerMethod
{
public:
	MapUnload(XmlRpcServer* s) : XmlRpcServerMethod("mapUnload", s) {}

	void execute(XmlRpcValue& params, XmlRpcValue& result)
	{
		std::stringstream r;
		r << g_mapmanager->unload(params[0]);
		result = r.str();
	}
} mapUnload(&s);

class MapGetDetail : public XmlRpcServerMethod
{
public:
	MapGetDetail(XmlRpcServer* s) : XmlRpcServerMethod("mapGetDetail", s) {}

	void execute(XmlRpcValue& params, XmlRpcValue& result)
	{
		std::string name = params[0];

		std::stringstream r;

		MapInfo dest;

		if (g_datasource->getMapInfo(name, dest))
		{
			r << "Name: " << dest.name << "<br/><br/>";
			r << "Autoload: " << (dest.autoload ? "Yes" : "No") << "<br/>";
			r << "<a href=\"rpc_map_set_autoload.php?autoload=true&name=" << dest.name << "\">Enable Autoload</a><br/>";
			r << "<a href=\"rpc_map_set_autoload.php?autoload=false&name=" << dest.name << "\">Disable Autoload</a><br/><br/>";
			r << "Loaded: " << (g_mapmanager->isLoaded(dest.name) ? "Yes" : "No") << "<br/>";
			r << "<a href=\"rpc_map_load.php?name=" << dest.name << "\">Load</a><br/>";
			r << "<a href=\"rpc_map_unload.php?name=" << dest.name << "\">Unload</a><br/>";
			r << "<br/><br/><br/>";
			r << "<a href=\"rpc_map_dispose.php?name=" << dest.name << "\">DETELE THIS MAP</a><br/>";
		}
		else
		{
			r << "Map doesn't exist.";
		}
		result = r.str();
	}
} mapGetDetail(&s);

class MapList : public XmlRpcServerMethod
{
public:
	MapList(XmlRpcServer* s) : XmlRpcServerMethod("mapList", s) {}

	void execute(XmlRpcValue& params, XmlRpcValue& result)
	{
		std::vector<std::pair<std::string, bool>> list;
		g_datasource->getMapList(list, false);

		std::stringstream r;

		r << "<maplist>\n";
		for (auto i : list)
		{
			r << "<map name=\"" << i.first << "\" autoload=\"" << i.second << "\" loaded=\"" << g_mapmanager->isLoaded(i.first) << "\"></map>\n";
		}
		r << "</maplist>\n";
		result = r.str();
	}
} mapList(&s);

class MapSetAutoLoad : public XmlRpcServerMethod
{
public:
	MapSetAutoLoad(XmlRpcServer* s) : XmlRpcServerMethod("mapSetAutoLoad", s) {}

	void execute(XmlRpcValue& params, XmlRpcValue& result)
	{
		std::stringstream r;
		r << g_datasource->setMapAutoLoad(params[0], params[1]);
		result = r.str();
	}
} mapSetAutoLoad(&s);

void runServer()
{
	std::cout << "Wait 1 sec for samp to init.\n";
	std::this_thread::sleep_for(std::chrono::seconds(1));
	g_datasource.reset(new DataSource("localhost"));
	g_mapmanager.reset(new MapManager(*g_datasource));
	g_mapmanager->loadAutoLoadMaps();

	int port = 7776;
	XmlRpc::setVerbosity(5);
	s.bindAndListen(port);
	s.enableIntrospection(true);
	s.work(-1.0);
}
