#pragma once 

#include <string>
#include <cstdint>

#include "AccountInfo.h"
#include "DataSource.h"

class Account
{
protected:
	AccountInfo info;
	DataSource& datasrc;

public:
	Account(const AccountInfo& ainfo, DataSource& db);

	std::string getLogName() const;
	std::string getNickname() const;
	std::string getUserID() const;
	bool setLogName(const std::string& name);
	bool setNickname(const std::string& name);

	int getMoney() const;
	void increaseMoney(int amount, const std::string& reason);

	bool isRegistered() const;
	bool isLoggedIn() const;
	bool create(const std::string& rawpw);
	// if success will change loggedin to true
	bool auth(const std::string& rawpw);
	bool changePassword(const std::string& newrawpw);

	int getAdminLevel() const;
	bool setAdminLevel(AccountInfo& operatorAccount, int level);

	int getInGameID() const;
	uint64_t getSessionID() const;
};
