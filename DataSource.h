#pragma once

#include <memory>

#include "MapInfo.h"
#include "AccountInfo.h"

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

	bool adminOperationLog(const std::string& operid, const std::string& effectedid, const std::string& operation, const std::string& msg);

	bool addBanRecord(AccountInfo& admin, const std::string& logname, const std::string& ip, const std::string& gpci);
	// notice: true if whether hash(gpci + ip) exists or hash(gpci + logname) exists.
	bool hasBanRecord(const std::string& logname, const std::string& ip, const std::string& gpci);

	bool makeSuggestion(AccountInfo& account, const std::string& content);
};
