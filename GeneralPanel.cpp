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

#include "GeneralPanel.h"

#include "AccountDialogs.h"
#include "WeaponShop.h"
#include "GameServer.h"
#include "StringUtility.h"
#include "AdminCommands.h"
#include "HouseDialogs.h"

using namespace std::placeholders;

void showGeneralPanel(const std::shared_ptr<Account>& player)
{
	DialogItemList list;
	list
		.append("�ҵ��˺�", std::bind(&showAccountOptionsDialog, player))
		.append("�ҵķ���", std::bind(&showHouseListDialog, player))
		.append("����", std::bind(&showWeaponShopDialog, player))
		.append("��������", std::bind(&showServerAdminDialog, player))
		.append("���������Ὠ��", [player]() {
			GameServer::getInstance().dialogmanager.displayInputDialog(
				player, "���������Ὠ��", "����������", "�ύ", "ȡ��", false,
				[player](const std::string& content) {
				GameServer::getInstance().datasource.makeSuggestion(player->_getAccountInfo(), content);
				SendClientMessage(player->getInGameID(), 0xFFFFFFFF, "[Server] ��л���Ľ���!");
			});})
		;

	GameServer::getInstance().dialogmanager.displayListDialog(
		player, STR(player->getLogName() << " �Ŀ������").c_str(), list, "ȷ��", "ȡ��");
}
