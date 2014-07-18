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
