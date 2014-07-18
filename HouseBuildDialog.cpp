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
