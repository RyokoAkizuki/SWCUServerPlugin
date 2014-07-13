#include "GeneralPanel.h"

#include "AccountDialogs.h"
#include "Shop.h"
#include "GameServer.h"
#include "StringUtility.h"
#include "AdminCommands.h"

void showGeneralPanel(const std::shared_ptr<Account>& player)
{
	DialogItemList list;
	list
		.append("我的账号", std::bind(&showAccountOptionsDialog, player))
		.append("商店", std::bind(&showShopDialog, player))
		.append("管理命令", std::bind(&showServerAdminDialog, player))
		;

	GameServer::getInstance().dialogmanager.displayListDialog(
		player, STR(player->getLogName() << " 的控制面板").c_str(), list, "确定", "取消");
}
