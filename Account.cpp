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

#include <sampgdk/a_players.h>
#include <ctime>

#include "StringUtility.h"
#include "Account.h"

Account::Account(const AccountInfo& ainfo, DataSource& db) : info(ainfo), datasrc(db)
{
	datasrc.loadAccount(info);
	info.logtime = time(NULL);
}

Account::~Account()
{
	if(info.loggedin) datasrc.increaseAccountPlayingTime(info, time(NULL) - info.logtime);
}

std::string Account::getLogName() const
{
	return info.logname;
}

std::string Account::getNickname() const
{
	return info.nickname;
}

std::string Account::getUserID() const
{
	return info.userid;
}

bool Account::setLogName(const std::string& name)
{
	std::string oldname = info.logname;

	int canchange = SetPlayerName(info.ingameid, name.c_str());

	switch (canchange)
	{
	case 1: // succeeded	
	{
		bool suc = datasrc.changeAccountLogName(info, name);
		if (!suc)
		{
			SendClientMessage(info.ingameid, 0xFFFFFFFF, "[Account] 数据库返回更改登录名称失败. 请联系管理员.");
			SetPlayerName(info.ingameid, oldname.c_str());
			return false;
		}
		SendClientMessage(info.ingameid, 0xFFFFFFFF, "[Account] 登录名更改成功.");
		return true;
	}
	case 0: // same
	{
		SendClientMessage(info.ingameid, 0xFFFFFFFF, "[Account] 要求更改的名称与正在使用的相同. 未进行更改.");
		return false;
	}
	case -1: // invalid
	{
		SendClientMessage(info.ingameid, 0xFFFFFFFF, "[Account] 要求更改的名称不符合要求. 请输入3-24个字符长由 0-9 a-z A-Z [] () $ @ . _  = 组成的有效名称.");
		return false;
	}
	}
	std::cout << "[Account] Warning: unexpected code reached in setLogName.\n";
	return false;
}

bool Account::setNickname(const std::string& name)
{
	if (datasrc.changeAccountNickName(info, name))
	{
		SendClientMessage(info.ingameid, 0xFFFFFFFF, "[Account] 昵称更改成功.");
		return true;
	}
	else
	{
		SendClientMessage(info.ingameid, 0xFFFFFFFF, "[Account] 昵称更改失败. 请联系管理员.");
		return false;
	}
}

int Account::getMoney() const
{
	return info.money;
}

void Account::increaseMoney(int amount, const std::string& reason)
{
	if (!datasrc.increaseAccountMoney(info, amount, reason))
	{
		SendClientMessage(info.ingameid, 0xFFFFFFFF, "[Account] 账单操作失败. 请联系管理员.");
		std::cout << "[Account] Warning: increaseAccountMoney Failed.\n";
	}
}

bool Account::isRegistered() const
{
	return info.registered;
}

bool Account::isLoggedIn() const
{
	return info.loggedin;
}

bool Account::create(const std::string& rawpw)
{
	if (datasrc.createAccount(info) && datasrc.changePassword(info, rawpw))
	{
		SendClientMessage(info.ingameid, 0xFFFFFFFF, "[Account] 注册成功.");
		return true;
	}
	else
	{
		SendClientMessage(info.ingameid, 0xFFFFFFFF, "[Account] 注册失败. 请联系管理员.");
		std::cout << "[Account] Warning: create Failed.\n";
		return false;
	}
}

bool Account::auth(const std::string& rawpw)
{
	if(datasrc.authAccount(info, rawpw))
	{
		SendClientMessage(info.ingameid, 0xFFFFFFFF, "[Account] 登录成功.");
		return true;
	}
	else
	{
		SendClientMessage(info.ingameid, 0xFFFFFFFF, "[Account] 登录失败. 请检查输入的密码.");
		return false;
	}
}

bool Account::changePassword(const std::string& newrawpw)
{
	if (newrawpw.length() < 6)
	{
		SendClientMessage(info.ingameid, 0xFFFFFFFF, "[Account] 请使用大于6位的密码.");
		return false;
	}
	if (datasrc.changePassword(info, newrawpw))
	{
		SendClientMessage(info.ingameid, 0xFFFFFFFF, "[Account] 密码更改成功.");
		return true;
	}
	else
	{
		SendClientMessage(info.ingameid, 0xFFFFFFFF, "[Account] 密码更改失败. 请联系管理员.");
		std::cout << "[Account] Warning: changePassword Failed.\n";
		return false;
	}
}

int Account::getAdminLevel() const
{
	return info.adminlevel;
}

bool Account::setAdminLevel(const std::shared_ptr<Account>& operatorAccount, int level)
{
	if (datasrc.changeAccountAdminLevel(info, operatorAccount->info, level))
	{
		SendClientMessageToAll(0xFFFFFFFF, STR("[Account] " << info.logname << UID(this) << " 的管理员等级被更改为 " << level << ".").c_str());
		return true;
	}
	else
	{
		std::cout << "[Account] Warning: changeAccountAdminLevel Failed.\n";
		return false;
	}
}

bool Account::isDisabled() const
{
	return info.disabled;
}

bool Account::setDisabled(bool disabled)
{
	if (datasrc.setAccountDisabled(info, disabled))
	{
		SendClientMessageToAll(0xFFFFFFFF, STR("[Account] 登录账号 " << info.logname << " 被" << (disabled ? "封禁" : "解禁") << ".").c_str());
		return true;
	}
	else
	{
		std::cout << "[Account] Warning: setDisabled Failed.\n";
		return false;
	}
}

int Account::getInGameID() const
{
	return info.ingameid;
}

uint64_t Account::getSessionID() const
{
	return info.session;
}

bool Account::isMuted() const
{
	return info.muted;
}

bool Account::isFreezed() const
{
	return info.freezed;
}

void Account::setMuted(bool muted)
{
	info.muted = muted;
}

void Account::setFreezed(bool freezed)
{
	info.freezed = freezed;
}

AccountInfo& Account::_getAccountInfo()
{
	return info;
}
