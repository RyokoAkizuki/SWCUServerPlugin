#include "DataSource.h"

std::string
	g_dbname_map = "swcuserver.map.brief",
	g_dbname_map_obj = "swcuserver.map.detail.object",
	g_dbname_map_veh = "swcuserver.map.detail.vehicle"
	;

DataSource::DataSource(const std::string& host) : conn(true)
{
	try
	{
		conn.connect(host);
		std::cout << "[DataSource] Connected to " << host << ".\n";
	}
	catch (const mongo::DBException &e)
	{
		std::cout << "[DataSource] Failed to connect to " << host << ". Caught " << e.what() << "\n";
		return;
	}

	try
	{
		conn.createCollection(g_dbname_map);
		conn.ensureIndex(g_dbname_map, BSON("name" << 1 << "autoload" << 1), true);

		conn.createCollection(g_dbname_map_obj);
		conn.ensureIndex(g_dbname_map_obj, BSON("mapid" << 1), false);

		conn.createCollection(g_dbname_map_veh);
		conn.ensureIndex(g_dbname_map_veh, BSON("mapid" << 1), false);

		std::cout << "[DataSource] Data collections are ready.\n";
	}
	catch (const mongo::DBException &e)
	{
		std::cout << "[DataSource] Collection check failed. Caught " << e.what() << "\n";
		return;
	}
}

bool DataSource::saveMap(const MapInfo& map, bool autoload)
{
	try
	{
		mongo::OID mapid(mongo::OID::gen());

		conn.insert(g_dbname_map, BSON("_id" << mapid << "name" << map.name << "date" << mongo::DATENOW << "autoload" << autoload));

		auto err = conn.getLastError();

		if (err.size())
		{
			std::cout << "[DataSource] Error occured when inserting map object: " << err << "\n";
			return false;
		}

		std::vector<mongo::BSONObj> objs;
		std::vector<mongo::BSONObj> vehs;

		for (auto i : map.objects)
		{
			objs.push_back(BSON(
				mongo::GENOID <<
				"mapid" << mapid <<
				"model" << i.model <<
				"x" << i.x <<
				"y" << i.y <<
				"z" << i.z <<
				"rx" << i.rx <<
				"ry" << i.ry <<
				"rz" << i.rz
				));
		}

		for (auto i : map.vehicles)
		{
			vehs.push_back(BSON(
				mongo::GENOID <<
				"mapid" << mapid <<
				"model" << i.model <<
				"x" << i.x <<
				"y" << i.y <<
				"z" << i.z <<
				"angle" << i.angle <<
				"color1" << i.color1 <<
				"color2" << i.color2
				));
		}

		conn.insert(g_dbname_map_obj, objs);
		conn.insert(g_dbname_map_veh, vehs);
	}
	catch (const mongo::DBException &e)
	{
		std::cout << "[DataSource] Map submitting failed. Caught " << e.what() << "\n";
		return false;
	}
	return true;
}

bool DataSource::disposeMap(const std::string& name)
{
	try
	{
		auto map = conn.findOne(g_dbname_map, QUERY("name" << name));
		if (map.isEmpty())
		{
			std::cout << "[DataSource] Map " << name << " not found. Disposing request ignored.\n";
			return false;
		}
		auto id = map["_id"].OID();
		conn.remove(g_dbname_map, QUERY("_id" << id), true);
		conn.remove(g_dbname_map_obj, BSON("mapid" << id));
		conn.remove(g_dbname_map_veh, BSON("mapid" << id));
	}
	catch (const mongo::DBException &e)
	{
		std::cout << "[DataSource] Map disposing failed. Caught " << e.what() << "\n";
		return false;
	}
	return true;
}

bool DataSource::loadMap(const std::string& name, MapInfo& dest)
{
	try
	{
		auto map = conn.findOne(g_dbname_map, QUERY("name" << name));
		if (map.isEmpty())
		{
			std::cout << "[DataSource] Map " << name << " not found. Can't load it.\n";
			return false;
		}
		auto id = map["_id"].OID();
		dest.name = map["name"].str();
		dest.autoload = map["autoload"].boolean();

		auto cur = conn.query(g_dbname_map_obj, QUERY("mapid" << id));
		while (cur->more())
		{
			auto obj = cur->next();
			dest.objects.push_back({
				obj["model"].numberInt(),
				obj["x"].numberDouble(),
				obj["y"].numberDouble(),
				obj["z"].numberDouble(),
				obj["rx"].numberDouble(),
				obj["ry"].numberDouble(),
				obj["rz"].numberDouble()
			});
		}

		cur = conn.query(g_dbname_map_veh, QUERY("mapid" << id));
		while (cur->more())
		{
			auto obj = cur->next();
			dest.vehicles.push_back({
				obj["model"].numberInt(),
				obj["x"].numberDouble(),
				obj["y"].numberDouble(),
				obj["z"].numberDouble(),
				obj["angle"].numberDouble(),
				obj["color1"].numberInt(),
				obj["color2"].numberInt()
			});
		}
	}
	catch (const mongo::DBException &e)
	{
		std::cout << "[DataSource] Map fetching failed. Caught " << e.what() << "\n";
		return false;
	}
	return true;
}
