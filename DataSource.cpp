#include "mongo/client/dbclient.h"

#include "DataSource.h"
#include "Account.h"
#include "EncodingUtility.h"
#include "StringUtility.h"

std::string g_dbname_map = "swcuserver.map.brief";
std::string g_dbname_map_obj = "swcuserver.map.detail.object";
std::string g_dbname_map_veh = "swcuserver.map.detail.vehicle";
std::string g_dbname_account = "swcuserver.account";
std::string g_dbname_account_admin = "swcuserver.account.admin";
std::string g_dbname_account_ban = "swcuserver.account.ban";
std::string g_dbname_bank_transfer = "swcuserver.bank.transfer";
std::string g_dbname_account_admin_log = "swcuserver.account.admin.log";
std::string g_dbname_suggestion = "swcuserver.suggestion";
std::string g_dbname_house = "swcuserver.house";
std::string g_dbname_house_trade = "swcuserver.house.trade";

DataSource::DataSource(const std::string& host) : conn(new mongo::DBClientConnection(true))
{
	try
	{
		conn->connect(host);
		std::cout << "[DataSource] Connected to " << host << ".\n";
	}
	catch (const mongo::DBException &e)
	{
		std::cout << "[DataSource] Failed to connect to " << host << ". Caught " << e.what() << "\n";
		return;
	}

	try
	{
		// Map
		conn->createCollection(g_dbname_map);
		conn->ensureIndex(g_dbname_map, BSON("name" << 1), true);
		conn->ensureIndex(g_dbname_map, BSON("autoload" << 1), false);

		conn->createCollection(g_dbname_map_obj);
		conn->ensureIndex(g_dbname_map_obj, BSON("mapid" << 1), false);

		conn->createCollection(g_dbname_map_veh);
		conn->ensureIndex(g_dbname_map_veh, BSON("mapid" << 1), false);

		//Account
		conn->createCollection(g_dbname_account);
		conn->ensureIndex(g_dbname_account, BSON("logname" << 1), true);
		conn->ensureIndex(g_dbname_account, BSON("password" << 1), false);
		conn->createCollection(g_dbname_account_admin);
		conn->createCollection(g_dbname_account_ban);
		conn->ensureIndex(g_dbname_account_ban, BSON("hash1" << 1 << "hash2" << 1), false);

		// Bank
		conn->createCollection(g_dbname_bank_transfer);
		conn->ensureIndex(g_dbname_bank_transfer, BSON("receiver" << 1 << "sender" << 1), false);

		// Server
		conn->createCollection(g_dbname_suggestion);

		// House
		conn->createCollection(g_dbname_house);
		conn->ensureIndex(g_dbname_house, BSON("ownerid" << 1), false);
		conn->createCollection(g_dbname_house_trade);

		std::cout << "[DataSource] Data collections are ready.\n";
	}
	catch (const mongo::DBException &e)
	{
		std::cout << "[DataSource] Collection check failed. Caught " << e.what() << "\n";
		return;
	}
}

// To prevent unique_ptr from complaining for complete definition of mongo::DBClientConnection
DataSource::~DataSource() {}

bool DataSource::saveMap(const MapInfo& map, bool autoload)
{
	try
	{
		mongo::OID mapid(mongo::OID::gen());

		conn->insert(g_dbname_map, BSON("_id" << mapid << "name" << map.name << "date" << mongo::DATENOW << "autoload" << autoload));

		auto err = conn->getLastError();

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

		conn->insert(g_dbname_map_obj, objs);
		conn->insert(g_dbname_map_veh, vehs);
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
		auto map = conn->findOne(g_dbname_map, QUERY("name" << name));
		if (map.isEmpty())
		{
			std::cout << "[DataSource] Map " << name << " not found. Disposing request ignored.\n";
			return false;
		}
		auto id = map["_id"].OID();
		conn->remove(g_dbname_map, QUERY("_id" << id), true);
		conn->remove(g_dbname_map_obj, BSON("mapid" << id));
		conn->remove(g_dbname_map_veh, BSON("mapid" << id));
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
		auto map = conn->findOne(g_dbname_map, QUERY("name" << name));
		if (map.isEmpty())
		{
			std::cout << "[DataSource] Map " << name << " not found. Can't load it.\n";
			return false;
		}
		auto id = map["_id"].OID();
		dest.name = map["name"].str();
		dest.autoload = map["autoload"].boolean();

		auto cur = conn->query(g_dbname_map_obj, QUERY("mapid" << id));
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

		cur = conn->query(g_dbname_map_veh, QUERY("mapid" << id));
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

void DataSource::getMapList(std::vector<std::pair<std::string, bool>>& list, bool onlyAutoLoad)
{
	try
	{
		std::cout << "[DataSource] Generating map list. OnlyAutoLoad = " << onlyAutoLoad << ".\n";

		auto mapcur = onlyAutoLoad ?
			conn->query(g_dbname_map, QUERY("autoload" << true)) :
			conn->query(g_dbname_map);

		
		while (mapcur->more())
		{
			auto obj = mapcur->next();
			list.push_back(std::make_pair(obj["name"].str(), obj["autoload"].boolean()));
		}

		std::cout << "[DataSource] Got a list of " << list.size() << " maps.\n";
	}
	catch (const mongo::DBException &e)
	{
		std::cout << "[DataSource] Error occured when gathering map list. Caught " << e.what() << "\n";
	}
}

bool DataSource::getMapInfo(const std::string& name, MapInfo& dest)
{
	try
	{
		auto map = conn->findOne(g_dbname_map, QUERY("name" << name));
		if (map.isEmpty())
		{
			std::cout << "[DataSource] Map " << name << " not found. Failed to get info.\n";
			return false;
		}
		dest.name = map["name"].str();
		dest.autoload = map["autoload"].boolean();
	}
	catch (const mongo::DBException &e)
	{
		std::cout << "[DataSource] Map fetching failed. Caught " << e.what() << "\n";
		return false;
	}
	return true;
}

bool DataSource::setMapAutoLoad(const std::string& name, bool autoload)
{
	try
	{
		conn->update(g_dbname_map, 
			QUERY("name" << name),
			BSON("$set" << BSON("autoload" << autoload))
			);
	}
	catch (const mongo::DBException &e)
	{
		std::cout << "[DataSource] Map fetching failed. Caught " << e.what() << "\n";
		return false; 
	}
	return true;
}

bool DataSource::loadAccount(AccountInfo& info)
{
	try
	{
		auto acc = conn->findOne(g_dbname_account, QUERY("logname" << GBKToUTF8(info.logname)));
		if (acc.isEmpty())
		{
			std::cout << "[DataSource] Account " << info.logname << " not found.\n";
			info.registered = false;
			return false;
		}
		info.registered = true;
		info.userid = acc["_id"].OID().str();
		info.logname = UTF8ToGBK(acc["logname"].str());
		info.nickname = UTF8ToGBK(acc["nickname"].str());
		info.money = acc["money"].numberInt();
		info.adminlevel = acc["adminlevel"].numberInt();
		info.disabled = acc["disabled"].boolean();
	}
	catch (const mongo::DBException &e)
	{
		std::cout << "[DataSource] Account fetch failed. Caught " << e.what() << "\n";
		return false;
	}
	return true;
}

bool DataSource::createAccount(AccountInfo& src)
{
	try
	{
		mongo::OID userid(mongo::OID::gen());

		conn->insert(g_dbname_account, BSON(
			"_id" << userid <<
			"logname" << GBKToUTF8(src.logname) <<
			"password" << hash_sha1(mongo::OID::gen().str()) << // random password. possibly near to userid.
			"jointime" << mongo::DATENOW <<
			"nickname" << GBKToUTF8(src.nickname) <<
			"money" << src.money <<
			"adminlevel" << src.adminlevel <<
			"disabled" << src.disabled <<
			"playingtime" << 0
			));

		auto err = conn->getLastError();

		if (err.size())
		{
			std::cout << "[DataSource] Error occured when inserting account object: " << err << "\n";
			return false;
		}
		
		src.userid = userid.str();
		src.loggedin = true;
	}
	catch (const mongo::DBException &e)
	{
		std::cout << "[DataSource] Account creating failed. Caught " << e.what() << "\n";
		return false;
	}
	return true;
}

bool DataSource::authAccount(AccountInfo& account, const std::string& rawpw)
{
	bool r = false;
	try
	{
		int count = conn->count(g_dbname_account, BSON(
			"_id" << mongo::OID(account.userid) << "password" << hash_sha1(GBKToUTF8(rawpw))
			));
		r = (count != 0);
	}
	catch (const mongo::DBException &e)
	{
		std::cout << "[DataSource] Account auth failed. Caught " << e.what() << "\n";
		r = false;
	}
	account.loggedin = r;
	return r;
}

bool DataSource::changePassword(const AccountInfo& account, const std::string& rawpw)
{
	try
	{
		conn->update(g_dbname_account,
			QUERY("_id" << mongo::OID(account.userid)),
			BSON("$set" << BSON("password" << hash_sha1(GBKToUTF8(rawpw))))
			);
	}
	catch (const mongo::DBException &e)
	{
		std::cout << "[DataSource] changePassword failed. Caught " << e.what() << "\n";
		return false;
	}
	return true;
}

bool DataSource::setAccountDisabled(AccountInfo& account, bool disabled)
{
	try
	{
		conn->update(g_dbname_account,
			QUERY("_id" << mongo::OID(account.userid)),
			BSON("$set" << BSON("disabled" << disabled))
			);
	}
	catch (const mongo::DBException &e)
	{
		std::cout << "[DataSource] setAccountDisabled failed. Caught " << e.what() << "\n";
		return false;
	}
	account.disabled = disabled;
	return true;
}

bool DataSource::changeAccountLogName(AccountInfo& account, const std::string& newname)
{
	try
	{
		conn->update(g_dbname_account,
			QUERY("_id" << mongo::OID(account.userid)),
			BSON("$set" << BSON("logname" << GBKToUTF8(newname)))
			);
	}
	catch (const mongo::DBException &e)
	{
		std::cout << "[DataSource] changeAccountLogName failed. Caught " << e.what() << "\n";
		return false;
	}
	account.logname = newname;
	return true;
}

bool DataSource::changeAccountNickName(AccountInfo& account, const std::string& newname)
{
	try
	{
		conn->update(g_dbname_account,
			QUERY("_id" << mongo::OID(account.userid)),
			BSON("$set" << BSON("nickname" << GBKToUTF8(newname)))
			);
	}
	catch (const mongo::DBException &e)
	{
		std::cout << "[DataSource] changeAccountNickName failed. Caught " << e.what() << "\n";
		return false;
	}
	account.nickname = newname;
	return true;
}

bool DataSource::increaseAccountMoney(AccountInfo& account, int amount, const std::string& reason)
{
	try
	{
		auto id = mongo::OID(account.userid);
		conn->update(g_dbname_account,
			QUERY("_id" << id),
			BSON("$inc" << BSON("money" << amount))
			);

		conn->insert(g_dbname_bank_transfer, BSON(
			mongo::GENOID <<
			"receiver" << id <<
			"sender" << mongo::OID() <<
			"time" << mongo::DATENOW <<
			"amount" << amount <<
			"reason" << reason
			));

		auto acc = conn->findOne(g_dbname_account, QUERY("_id" << id));
		account.money = acc["money"].numberInt();
	}
	catch (const mongo::DBException &e)
	{
		std::cout << "[DataSource] increaseAccountMoney failed. Caught " << e.what() << "\n";
		return false;
	}
	return true;
}

bool DataSource::changeAccountAdminLevel(AccountInfo& account, const AccountInfo& oper, int level)
{
	try
	{
		auto id = mongo::OID(account.userid);

		conn->update(g_dbname_account,
			QUERY("_id" << id),
			BSON("$set" << BSON("adminlevel" << level))
			);

		conn->insert(g_dbname_account_admin, BSON(
			mongo::GENOID <<
			"userid" << id <<
			"operator" << mongo::OID(oper.userid) <<
			"time" << mongo::DATENOW <<
			"level" << level
			));

		account.adminlevel = level;
	}
	catch (const mongo::DBException &e)
	{
		std::cout << "[DataSource] changeAccountAdminLevel failed. Caught " << e.what() << "\n";
		return false;
	}
	return true;
}

bool DataSource::increaseAccountPlayingTime(AccountInfo& account, int64_t time)
{
	try
	{
		auto id = mongo::OID(account.userid);
		conn->update(g_dbname_account,
			QUERY("_id" << id),
			BSON("$inc" << BSON("playingtime" << time))
			);
	}
	catch (const mongo::DBException &e)
	{
		std::cout << "[DataSource] increaseAccountPlayingTime failed. Caught " << e.what() << "\n";
		return false;
	}
	return true;
}

bool DataSource::searchAccounts(const std::string& partofname, std::vector<std::pair<std::string, std::string>>& namedest)
{
	try
	{
		auto cur = conn->query(g_dbname_account, QUERY("logname" << BSON(
			"$regex" << STR(".*" << GBKToUTF8(partofname) << ".*") << "$options" << "i")));

		while (cur->more())
		{
			auto obj = cur->next();
			namedest.push_back(std::make_pair(obj["_id"].OID().str(), UTF8ToGBK(obj["logname"].str())));
		}
	}
	catch (const mongo::DBException &e)
	{
		std::cout << "[DataSource] searchAccounts failed. Caught " << e.what() << "\n";
		return false;
	}
	return true;
}

bool DataSource::adminOperationLog(const std::string& operid, const std::string& effectedid, const std::string& operation, const std::string& msg)
{
	try
	{
		conn->insert(g_dbname_account_admin_log, BSON(
			mongo::GENOID <<
			"operator" << mongo::OID(operid) <<
			"effected" << mongo::OID(effectedid) <<
			"operation" << GBKToUTF8(operation) <<
			"message" << GBKToUTF8(msg) <<
			"time" << mongo::DATENOW
			));
	}
	catch (const mongo::DBException &e)
	{
		std::cout << "[DataSource] adminOperationLog failed. Caught " << e.what() << "\n";
		return false;
	}
	return true;
}

bool DataSource::addBanRecord(AccountInfo& admin, const std::string& logname, const std::string& ip, const std::string& gpci)
{
	try
	{
		conn->insert(g_dbname_account_ban, BSON(
			mongo::GENOID <<
			"hash1" << hash_sha1(gpci + GBKToUTF8(logname)) <<
			"hash2" << hash_sha1(gpci + ip) <<
			"gpci" << gpci <<
			"logname" << GBKToUTF8(logname) <<
			"ip" << ip <<
			"time" << mongo::DATENOW <<
			"admin" << admin.userid
			));
	}
	catch (const mongo::DBException &e)
	{
		std::cout << "[DataSource] addBanRecord failed. Caught " << e.what() << "\n";
		return false;
	}
	return true;
}

bool DataSource::hasBanRecord(const std::string& logname, const std::string& ip, const std::string& gpci)
{
	try
	{
		int count = conn->count(g_dbname_account_ban, BSON(
			"$or" << BSON_ARRAY(
				BSON("hash1" << hash_sha1(gpci + GBKToUTF8(logname))) <<
				BSON("hash2" << hash_sha1(gpci + ip))
			)
		));
		return count != 0;
	}
	catch (const mongo::DBException &e)
	{
		std::cout << "[DataSource] hasBanRecord failed. Caught " << e.what() << "\n";
		return false;
	}
	return false;
}

bool DataSource::makeSuggestion(AccountInfo& account, const std::string& content)
{
	try
	{
		conn->insert(g_dbname_suggestion, BSON(
			mongo::GENOID <<
			"userid" << mongo::OID(account.userid) <<
			"time" << mongo::DATENOW <<
			"content" << GBKToUTF8(content)
			));
	}
	catch (const mongo::DBException &e)
	{
		std::cout << "[DataSource] makeSuggestion failed. Caught " << e.what() << "\n";
		return false;
	}
	return true;
}

bool DataSource::createHouse(HouseInfo& house)
{
	try
	{
		mongo::OID houseid(mongo::OID::gen());

		conn->insert(g_dbname_house, BSON(
			"_id" << houseid <<
			"ownerid" << mongo::OID(house.ownerid) <<
			"name" << GBKToUTF8(house.name) <<
			"password" << GBKToUTF8(house.password) <<
			"time" << mongo::DATENOW <<
			"bindedmapname" << GBKToUTF8(house.bindedmapname) <<
			"ex" << house.ex << "ey" << house.ey << "ez" << house.ez << "rotation" << house.rotation <<
			"tx" << house.tx << "ty" << house.ty << "tz" << house.tz <<
			"lx" << house.lx << "ly" << house.ly << "lz" << house.lz <<
			"mx" << house.mx << "my" << house.my << "mz" << house.mz <<
			"interior" << house.interior <<
			"price" << house.price << "onsale" << house.onsale << "rent" << house.rent << "expiredtime" << house.expiredtime
			));

		auto err = conn->getLastError();

		if (err.size())
		{
			std::cout << "[DataSource] createHouse failed: " << err << "\n";
			return false;
		}

		house.houseid = houseid.str();
	}
	catch (const mongo::DBException &e)
	{
		std::cout << "[DataSource] createHouse failed. Caught " << e.what() << "\n";
		return false;
	}
	return true;
}

void DataSource::loadHouses(std::vector<HouseInfo>& dest, const std::string& playerid)
{
	try
	{
		auto cur = (playerid.empty()) ?
			conn->query(g_dbname_house) :
			conn->query(g_dbname_house, QUERY("ownerid" << mongo::OID(playerid)));

		HouseInfo info;

		while (cur->more())
		{
			auto obj = cur->next();
			info.houseid = obj["_id"].OID().str();
			info.ownerid = obj["ownerid"].OID().str();
			info.name = UTF8ToGBK(obj["name"].str());
			info.password = UTF8ToGBK(obj["password"].str());
			info.bindedmapname = UTF8ToGBK(obj["bindedmapname"].str());
			info.ex = obj["ex"].numberDouble(); info.ey = obj["ey"].numberDouble(); info.ez = obj["ez"].numberDouble();
			info.tx = obj["tx"].numberDouble(); info.ty = obj["ty"].numberDouble(); info.tz = obj["tz"].numberDouble();
			info.lx = obj["lx"].numberDouble(); info.ly = obj["ly"].numberDouble(); info.lz = obj["lz"].numberDouble();
			info.mx = obj["mx"].numberDouble(); info.my = obj["my"].numberDouble(); info.mz = obj["mz"].numberDouble();
			info.rotation = obj["rotation"].numberDouble();
			info.interior = obj["interior"].numberInt();
			info.price = obj["price"].numberInt();
			info.onsale = obj["onsale"].boolean();
			info.rent = obj["rent"].boolean();
			info.expiredtime = obj["expiredtime"].numberLong();

			dest.push_back(info);
		}
	}
	catch (const mongo::DBException &e)
	{
		std::cout << "[DataSource] loadHouses failed. Caught " << e.what() << "\n";
	}
}

bool DataSource::loadHouse(const std::string& houseid, HouseInfo& info)
{
	try
	{
		auto obj = conn->findOne(g_dbname_house, QUERY("_id" << mongo::OID(houseid)));
		if (obj.isEmpty())
		{
			std::cout << "[DataSource] loadHouse failed: id not found.\n";
			return false;
		}
		info.houseid = obj["_id"].OID().str();
		info.ownerid = obj["ownerid"].OID().str();
		info.name = UTF8ToGBK(obj["name"].str());
		info.password = UTF8ToGBK(obj["password"].str());
		info.bindedmapname = UTF8ToGBK(obj["bindedmapname"].str());
		info.ex = obj["ex"].numberDouble(); info.ey = obj["ey"].numberDouble(); info.ez = obj["ez"].numberDouble();
		info.tx = obj["tx"].numberDouble(); info.ty = obj["ty"].numberDouble(); info.tz = obj["tz"].numberDouble();
		info.lx = obj["lx"].numberDouble(); info.ly = obj["ly"].numberDouble(); info.lz = obj["lz"].numberDouble();
		info.mx = obj["mx"].numberDouble(); info.my = obj["my"].numberDouble(); info.mz = obj["mz"].numberDouble();
		info.rotation = obj["rotation"].numberDouble();
		info.interior = obj["interior"].numberInt();
		info.price = obj["price"].numberInt();
		info.onsale = obj["onsale"].boolean();
		info.rent = obj["rent"].boolean();
		info.expiredtime = obj["expiredtime"].numberLong();
	}
	catch (const mongo::DBException &e)
	{
		std::cout << "[DataSource] loadHouse failed. Caught " << e.what() << "\n";
		return false;
	}
	return true;
}

bool DataSource::setHouseExpiredTime(HouseInfo& house, int64_t time)
{
	try
	{
		conn->update(g_dbname_house,
			QUERY("_id" << mongo::OID(house.houseid)),
			BSON("$set" << BSON("expiredtime" << time)));
	}
	catch (const mongo::DBException &e)
	{
		std::cout << "[DataSource] setHouseExpiredTime failed. Caught " << e.what() << "\n";
		return false;
	}
	house.expiredtime = time;
	return true;
}

bool DataSource::setHouseOwner(HouseInfo& house, const std::string& ownerid)
{
	try
	{
		conn->update(g_dbname_house,
			QUERY("_id" << mongo::OID(house.houseid)),
			BSON("$set" << BSON("ownerid" << mongo::OID(ownerid))));
	}
	catch (const mongo::DBException &e)
	{
		std::cout << "[DataSource] setHouseOwner failed. Caught " << e.what() << "\n";
		return false;
	}
	house.ownerid = ownerid;
	return true;
}

bool DataSource::setHouseEntrance(HouseInfo& house, float x, float y, float z, float rotation)
{
	try
	{
		conn->update(g_dbname_house,
			QUERY("_id" << mongo::OID(house.houseid)),
			BSON("$set" << BSON("ex" << x << "ey" << y << "ez" << z << "rotation" << rotation)));
	}
	catch (const mongo::DBException &e)
	{
		std::cout << "[DataSource] setHouseEntrance failed. Caught " << e.what() << "\n";
		return false;
	}
	house.ex = x; house.ey = y; house.ez = z; house.rotation = rotation;
	return true;
}

bool DataSource::setHouseTeleportPos(HouseInfo& house, float x, float y, float z)
{
	try
	{
		conn->update(g_dbname_house,
			QUERY("_id" << mongo::OID(house.houseid)),
			BSON("$set" << BSON("tx" << x << "ty" << y << "tz" << z)));
	}
	catch (const mongo::DBException &e)
	{
		std::cout << "[DataSource] setHouseTeleportPos failed. Caught " << e.what() << "\n";
		return false;
	}
	house.tx = x; house.ty = y; house.tz = z;
	return true;
}

bool DataSource::setHouseName(HouseInfo& house, const std::string& name)
{
	try
	{
		conn->update(g_dbname_house,
			QUERY("_id" << mongo::OID(house.houseid)),
			BSON("$set" << BSON("name" << GBKToUTF8(name)))
			);
	}
	catch (const mongo::DBException &e)
	{
		std::cout << "[DataSource] changeAccountNickName failed. Caught " << e.what() << "\n";
		return false;
	}
	house.name = name;
	return true;
}

bool DataSource::setHousePassword(HouseInfo& house, const std::string& password)
{
	try
	{
		conn->update(g_dbname_house,
			QUERY("_id" << mongo::OID(house.houseid)),
			BSON("$set" << BSON("password" << GBKToUTF8(password)))
			);
	}
	catch (const mongo::DBException &e)
	{
		std::cout << "[DataSource] setHousePassword failed. Caught " << e.what() << "\n";
		return false;
	}
	house.password = password;
	return true;
}
