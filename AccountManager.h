#pragma once

#include <map>
#include <memory>

#include "Account.h"
#include "DataSource.h"

class AccountManager
{
protected:
	// <ingameid, info>
	std::map<int, std::shared_ptr<Account>> accounts;
	DataSource& datasource;

	void _buildFakeAccount();

public:
	AccountManager(DataSource& db);

	// Returns fake account if not found.
	std::shared_ptr<Account> findAccount(int ingameid);
	std::shared_ptr<Account> enterServer(int ingameid);
	void exitServer(int ingameid);
};
