#include <sampgdk/a_players.h>

#include "GeneralPanel.h"

#include "AccountDialogs.h"
#include "WeaponShop.h"
#include "GameServer.h"
#include "StringUtility.h"
#include "AdminCommands.h"

using namespace std::placeholders;

void showGeneralPanel(const std::shared_ptr<Account>& player)
{
	DialogItemList list;
	list
		.append("我的账号", std::bind(&showAccountOptionsDialog, player))
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
