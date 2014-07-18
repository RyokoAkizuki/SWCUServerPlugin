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

#include "Account.h"

enum AdminCommandType
{
	ADMIN_TARGET_REASON,
	ADMIN_TARGET,
	ADMIN_REASON,
	ADMIN
};

class AdminCommandBase
{
public:
	std::string name;
	int requiredlevel;
	AdminCommandType type;

public:
	AdminCommandBase(const std::string name, int reqlevel, AdminCommandType type, std::vector<AdminCommandBase*>& registration) :
		name(name), requiredlevel(reqlevel), type(type)
	{
		registration.push_back(this);
	}

	virtual void executeA(const std::shared_ptr<Account>& admin) {}
	virtual void executeAT(const std::shared_ptr<Account>& admin, const std::shared_ptr<Account>& target) {}
	virtual void executeAR(const std::shared_ptr<Account>& admin, const std::string& reason) {}
	virtual void executeATR(const std::shared_ptr<Account>& admin, const std::shared_ptr<Account>& target, const std::string& reason) {}
};

void showAdminDialog(const std::shared_ptr<Account>& admin, const std::shared_ptr<Account>& target, std::vector<AdminCommandBase*>& cmdlist);
void showPlayerAdminDialog(const std::shared_ptr<Account>& admin, const std::shared_ptr<Account>& target);
void showServerAdminDialog(const std::shared_ptr<Account>& admin);
void showAdminInputDialog(const std::shared_ptr<Account>& player, const std::function<void(const std::string&)>& callback);
