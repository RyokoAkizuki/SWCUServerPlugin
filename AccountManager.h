#pragma once

#include <map>

#include "Account.h"
#include "DataSource.h"

class AccountManager
{
protected:
	// <ingameid, info>
	std::map<int, Account> accounts;
	DataSource& datasource;

public:
	AccountManager(DataSource& db);

	// Returns fake account if not found.
	Account& findAccount(int ingameid);
	Account& enterServer(int ingameid);
	void exitServer(int ingameid);
};
