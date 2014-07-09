#include <XmlRpc.h>

#include <iostream>
#include <sstream>
#include <algorithm>
#include <memory>

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

void runServer()
{
	g_datasource.reset(new DataSource("localhost"));
	g_mapmanager.reset(new MapManager(*g_datasource));
	g_mapmanager->loadAutoLoadMaps();

	int port = 7776;
	XmlRpc::setVerbosity(5);
	s.bindAndListen(port);
	s.enableIntrospection(true);
	s.work(-1.0);
}
