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

#include "HouseBuildDialog.h"
#include "GameServer.h"
#include "AccountDialogs.h"
#include "StringUtility.h"

void showHouseBuildSetOwnerDialog(const std::shared_ptr<Account>& player, const std::shared_ptr<HouseBuilder>& session)
{
	showSearchPlayerDialog(player, [player, session](const std::string& id) {
		session->setOwner(id);
		SendClientMessage(player->getInGameID(), 0xFFFFFFFF, "[HouseBuilder] 房主设置成功");
		showHouseBuildSetNameDialog(player, session);
	});
}

void showHouseBuildSetNameDialog(const std::shared_ptr<Account>& player, const std::shared_ptr<HouseBuilder>& session)
{
	GameServer::getInstance().dialogmanager.displayInputDialog(player, "输入房屋名称", " ",
		"确定", "取消", false, [player, session](const std::string& name) {
		session->setName(name);
		SendClientMessage(player->getInGameID(), 0xFFFFFFFF, STR("[HouseBuilder] 房屋名称设置为 " << name).c_str());
		SendClientMessage(player->getInGameID(), 0xFFFFFFFF, "[HouseBuilder] 现在请绕房屋走一圈以确定边界 按鼠标左键开始记录 按鼠标右键停止记录 按鼠标中键完成");
		session->startUpdateArea();
	});
}
