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
		.append("我的账号", std::bind(&showAccountOptionsDialog, player))
		.append("我的房屋", std::bind(&showHouseListDialog, player))
		.append("武器", std::bind(&showWeaponShopDialog, player))
		.append("管理命令", std::bind(&showServerAdminDialog, player))
		.append("给服务器提建议", [player]() {
			GameServer::getInstance().dialogmanager.displayInputDialog(
				player, "给服务器提建议", "请输入内容", "提交", "取消", false,
				[player](const std::string& content) {
				GameServer::getInstance().datasource.makeSuggestion(player->_getAccountInfo(), content);
				SendClientMessage(player->getInGameID(), 0xFFFFFFFF, "[Server] 感谢您的建议!");
			});})
		;

	GameServer::getInstance().dialogmanager.displayListDialog(
		player, STR(player->getLogName() << " 的控制面板").c_str(), list, "确定", "取消");
}
