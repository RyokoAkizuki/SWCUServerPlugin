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
		.append("�ҵ��˺�", std::bind(&showAccountOptionsDialog, player))
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
