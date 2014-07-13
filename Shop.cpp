#include "Shop.h"
#include "GameServer.h"

#include "WeaponShop.h"

void showShopDialog(const std::shared_ptr<Account>& player)
{
	DialogItemList list;
	list
		.append("����", std::bind(&showWeaponShopDialog, player))
		;

	GameServer::getInstance().dialogmanager.displayListDialog(
		player, "�̵�", list, "ȷ��", "ȡ��");
}
