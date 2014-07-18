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
	virtual ~Account();

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
	bool setAdminLevel(const std::shared_ptr<Account>& operatorAccount, int level);

	bool isDisabled() const;
	bool setDisabled(bool disabled);

	// In-Game info

	int getInGameID() const;
	uint64_t getSessionID() const;
	bool isMuted() const;
	bool isFreezed() const;
	void setMuted(bool muted);
	void setFreezed(bool freezed);

	AccountInfo& _getAccountInfo();
};
