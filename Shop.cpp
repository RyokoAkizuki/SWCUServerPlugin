#include "Shop.h"
#include "GameServer.h"

#include "WeaponShop.h"

void showShopDialog(const std::shared_ptr<Account>& player)
{
	DialogItemList list;
	list
		.append("武器", std::bind(&showWeaponShopDialog, player))
		;

	GameServer::getInstance().dialogmanager.displayListDialog(
		player, "商店", list, "确定", "取消");
}
