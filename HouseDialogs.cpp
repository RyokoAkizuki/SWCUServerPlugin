#include <sampgdk/a_samp.h>
#include <sampgdk/a_players.h>

#include "HouseDialogs.h"
#include "Teleport.h"
#include "GameServer.h"

void showHouseAuthDialog(const std::shared_ptr<Account>& player, const std::shared_ptr<House>& house)
{
	GameServer::getInstance().dialogmanager.displayInputDialog(player, "������������", "����������", "����", "�뿪", false,
	[player, house](const std::string& password) {
		if (house->auth(password))
		{
			SendClientMessage(player->getInGameID(), 0xFFFFFFFF, "[House] ��֤�ɹ�");
		}
		else
		{
			SendClientMessage(player->getInGameID(), 0xFFFFFFFF, "[House] �������");
			showHouseAuthDialog(player, house);
		}
	}, [player, house](const std::string& password) { teleportToPos(player->getInGameID(), house->_getInfo().ex, house->_getInfo().ey, house->_getInfo().ez + 1, 0, 0); });
}

void showHouseListDialog(const std::shared_ptr<Account>& player)
{
	std::vector<HouseInfo> dest;
	DialogItemList list;
	GameServer::getInstance().datasource.loadHouses(dest, player->getUserID());
	for (auto& i : dest)
	{
		list.append(i.name, [i, player](){
			showHouseManageDialog(player, GameServer::getInstance().housemanager.findHouse(i.houseid));
		});
	}
	GameServer::getInstance().dialogmanager.displayListDialog(player, "�ҵķ���", list, "�鿴", "ȡ��");
}

void showHouseManageDialog(const std::shared_ptr<Account>& player, const std::shared_ptr<House>& house)
{
	DialogItemList list;
	list
		.append("��������", std::bind(&showHouseChangeNameDialog, player, house))
		.append("���͵��ſ�", [player, house]() {
			teleportToPos(player->getInGameID(), house->_getInfo().ex, house->_getInfo().ey, house->_getInfo().ez + 1, 0, 0);
		})
		;

	GameServer::getInstance().dialogmanager.displayListDialog(
		player, house->getName(), list, "ȷ��", "ȡ��");
}

void showHouseChangeNameDialog(const std::shared_ptr<Account>& player, const std::shared_ptr<House>& house)
{
	GameServer::getInstance().dialogmanager.displayInputDialog(player, "���ķ�������", "�������µ�����",
		"ȷ��", "ȡ��", false,
		[house](const std::string& newname) { house->setName(newname); }
	);
}

