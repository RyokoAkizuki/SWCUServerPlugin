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
#include <sstream>

#include "AccountDialogs.h"
#include "GameServer.h"
#include "StringUtility.h"

void showLoginDialog(const std::shared_ptr<Account>& player)
{
	GameServer::getInstance().dialogmanager.displayInputDialog(player, "��¼ Login", "����������\nPlease enter your password\n[OK] [Exit]",
		"��¼", "�˳�������", false,
		[player](const std::string& pw) {
			if (!player->auth(pw))
			{
				showLoginDialog(player);
			}
		},
		[player](const std::string& pw) { Kick(player->getInGameID()); }
	);
}

void showRegisterDialog(const std::shared_ptr<Account>& player)
{
	GameServer::getInstance().dialogmanager.displayInputDialog(player, "ע�� Register", "���������6λ������\nPlease use a password longer than 6 chars.\n[OK] [Exit]",
		"ע��", "�˳�������", false,
		[player](const std::string& pw) { player->create(pw); },
		[player](const std::string& pw) { Kick(player->getInGameID()); }
	);
}

void showChangePasswordDialog(const std::shared_ptr<Account>& player)
{
	GameServer::getInstance().dialogmanager.displayInputDialog(player, "��������", "���������6λ������",
		"ȷ��", "ȡ��", false,
		[player](const std::string& pw) { player->changePassword(pw); }
	);
}

void showSetLogNameDialog(const std::shared_ptr<Account>& player)
{
	GameServer::getInstance().dialogmanager.displayInputDialog(player, "���ĵ�¼��", "�������µ�����",
		"ȷ��", "ȡ��", false,
		[player](const std::string& newname) { player->setLogName(newname); }
	);
}

void showSetNicknameDialog(const std::shared_ptr<Account>& player)
{
	GameServer::getInstance().dialogmanager.displayInputDialog(player, "������ʾ��", "�������µ�����",
		"ȷ��", "ȡ��", false,
		[player](const std::string& name) { player->setNickname(name); }
	);
}

void showViewAccountDetailDialog(const std::shared_ptr<Account>& player)
{
	std::stringstream str;
	str << "ID: " << player->getUserID() <<
		"\n��¼��: " << player->getLogName() <<
		"\n��ʾ��: " << player->getNickname() <<
		"\n���п����: " << player->getMoney() <<
		"\n����Ա�ȼ�: " << player->getAdminLevel();
	GameServer::getInstance().dialogmanager.displayMessageDialog(player,
		"�ҵ���Ϣ", str.str(), "ȷ��", "");
}

void showAccountOptionsDialog(const std::shared_ptr<Account>& player)
{
	DialogItemList list;
	list
		.append("�鿴�ҵ���Ϣ", std::bind(&showViewAccountDetailDialog, player))
		.append("��������", std::bind(&showChangePasswordDialog, player))
		.append("���ĵ�¼��", std::bind(&showSetLogNameDialog, player))
		.append("������ʾ��", std::bind(&showSetNicknameDialog, player))
		;

	GameServer::getInstance().dialogmanager.displayListDialog(
		player, "�ҵ��˺�", list, "ȷ��", "ȡ��");
}

void showSearchPlayerDialog(const std::shared_ptr<Account>& player, const std::function<void(const std::string&)>& cb)
{
	GameServer::getInstance().dialogmanager.displayInputDialog(player, "�������", "�������ȫ���򲿷���",
		"����", "ȡ��", false,
	[player, cb](const std::string& name) { 
		std::vector<std::pair<std::string, std::string>> dest;
		GameServer::getInstance().datasource.searchAccounts(name, dest);
		DialogItemList list;
		for (auto& i : dest)
		{
			list.append(STR(i.first << " " << i.second), std::bind(cb, i.first));
		}
		GameServer::getInstance().dialogmanager.displayListDialog(player, STR("������� " << name), list, "�鿴", "ȡ��");
	}
	);
}
