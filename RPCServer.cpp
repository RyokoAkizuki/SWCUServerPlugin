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

#include <XmlRpc.h>

#include <iostream>
#include <sstream>
#include <algorithm>

#include "GameServer.h"
#include "MapCodeParser.h"

using namespace XmlRpc;

XmlRpcServer s;

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
		r << GameServer::getInstance().mapmanager.save(dest, params[2]);
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
		r << GameServer::getInstance().mapmanager.dispose(params[0]);
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
		r << GameServer::getInstance().mapmanager.load(params[0]);
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
		r << GameServer::getInstance().mapmanager.unload(params[0]);
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

		if (GameServer::getInstance().datasource.getMapInfo(name, dest))
		{
			r << "Name: " << dest.name << "<br/><br/>";
			r << "Autoload: " << (dest.autoload ? "Yes" : "No") << "<br/>";
			r << "<a href=\"rpc_map_set_autoload.php?autoload=true&name=" << dest.name << "\">Enable Autoload</a><br/>";
			r << "<a href=\"rpc_map_set_autoload.php?autoload=false&name=" << dest.name << "\">Disable Autoload</a><br/><br/>";
			r << "Loaded: " << (GameServer::getInstance().mapmanager.isLoaded(dest.name) ? "Yes" : "No") << "<br/>";
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
		GameServer::getInstance().datasource.getMapList(list, false);

		std::stringstream r;

		r << "<maplist>\n";
		for (auto i : list)
		{
			r << "<map name=\"" << i.first << "\" autoload=\"" << i.second << "\" loaded=\"" << GameServer::getInstance().mapmanager.isLoaded(i.first) << "\"></map>\n";
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
		r << GameServer::getInstance().datasource.setMapAutoLoad(params[0], params[1]);
		result = r.str();
	}
} mapSetAutoLoad(&s);

void runServer()
{
	int port = 7776;
	XmlRpc::setVerbosity(5);
	s.bindAndListen(port);
	s.enableIntrospection(true);
	s.work(-1.0);
}
