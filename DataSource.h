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

#include <memory>

#include "MapInfo.h"
#include "AccountInfo.h"
#include "HouseInfo.h"

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
	void getMapList(std::vector<std::pair<std::string, bool>>& dest, bool onlyAutoLoad = false);
	bool getMapInfo(const std::string& name, MapInfo& dest);
	bool setMapAutoLoad(const std::string& name, bool autoload);
	
	// Notice: this wlll update info.
	bool loadAccount(AccountInfo& info);
	// Notice: this will update userid value in accountinfo.
	bool createAccount(AccountInfo& src);
	// Notice: this will update loggedin value in accountinfo.
	bool authAccount(AccountInfo& account, const std::string& rawpw);
	bool changePassword(const AccountInfo& account, const std::string& rawpw);
	// Notice: this will update disabled value in accountinfo.
	bool setAccountDisabled(AccountInfo& account, bool disabled);
	// Notice: this will update logname value in accountinfo.
	bool changeAccountLogName(AccountInfo& account, const std::string& newname);
	// Notice: this will update nickname value in accountinfo.
	bool changeAccountNickName(AccountInfo& account, const std::string& newname);
	// Notice: this will update money value in accountinfo.
	bool increaseAccountMoney(AccountInfo& account, int amount, const std::string& reason);
	// Notice: this will update adminlevel value in accountinfo.
	bool changeAccountAdminLevel(AccountInfo& account, const AccountInfo& oper, int level);
	bool increaseAccountPlayingTime(AccountInfo& account, int64_t time);
	// <id, logname>
	bool searchAccounts(const std::string& partofname, std::vector<std::pair<std::string, std::string>>& namedest);

	bool adminOperationLog(const std::string& operid, const std::string& effectedid, const std::string& operation, const std::string& msg);

	bool addBanRecord(AccountInfo& admin, const std::string& logname, const std::string& ip, const std::string& gpci);
	// notice: true if whether hash(gpci + ip) exists or hash(gpci + logname) exists.
	bool hasBanRecord(const std::string& logname, const std::string& ip, const std::string& gpci);

	bool makeSuggestion(AccountInfo& account, const std::string& content);

	bool createHouse(HouseInfo& house);
	void loadHouses(std::vector<HouseInfo>& dest, const std::string& playerid = "");
	bool loadHouse(const std::string& houseid, HouseInfo& info);
	bool setHouseExpiredTime(HouseInfo& house, int64_t time);
	bool setHouseOwner(HouseInfo& house, const std::string& ownerid);
	bool setHouseEntrance(HouseInfo& house, float x, float y, float z, float rotation);
	bool setHouseTeleportPos(HouseInfo& house, float x, float y, float z);
	bool setHouseName(HouseInfo& house, const std::string& name);
	bool setHousePassword(HouseInfo& house, const std::string& password);
};
