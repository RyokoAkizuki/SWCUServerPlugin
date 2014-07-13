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
		.append("�ҵ��˺�", std::bind(&showAccountOptionsDialog, player))
		.append("�̵�", std::bind(&showShopDialog, player))
		.append("��������", std::bind(&showServerAdminDialog, player))
		;

	GameServer::getInstance().dialogmanager.displayListDialog(
		player, STR(player->getLogName() << " �Ŀ������").c_str(), list, "ȷ��", "ȡ��");
}
