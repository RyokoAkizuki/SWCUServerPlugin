#include "AccountManager.h"
#include <ctime>
#include <sampgdk/a_players.h>

AccountManager::AccountManager(DataSource& db) : datasource(db)
{
	accounts.insert(std::make_pair(-1, Account(AccountInfo(), datasource)));
}

Account& AccountManager::findAccount(int ingameid)
{
	auto iter = accounts.find(ingameid);
	if (iter == accounts.end()) // not found
	{
		iter = accounts.find(-1); // find the fake one
		if (iter == accounts.end()) // fake one is missing
		{
			iter = accounts.insert(std::make_pair(-1, Account(AccountInfo(), datasource))).first;
		}
	}
	return iter->second;
}

Account& AccountManager::enterServer(int ingameid)
{
	AccountInfo info;
	info.ingameid = ingameid;
	info.session = clock();
	char name[24];
	GetPlayerName(info.ingameid, name, 24);
	info.logname = name;

	auto r = accounts.insert(std::make_pair(ingameid, Account(info, datasource)));
	if (r.first == accounts.end())
	{
		std::cout << "[AccountManager] Warning: enterServer failed.\n";
		return findAccount(-1);
	}
	return r.first->second;
}

void AccountManager::exitServer(int ingameid)
{
	accounts.erase(ingameid);
}
