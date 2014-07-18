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
	}, [player, house](const std::string& password) { teleportToPos(player->getInGameID(), house->_getInfo().tx, house->_getInfo().ty, house->_getInfo().tz + 1, 0, 0); });
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
		.append("�����Ʒ�����վ��λ��", [player, house]() {
			float pos[3], rotation;
			GetPlayerPos(player->getInGameID(), &pos[0], &pos[1], &pos[2]);
			pos[2] -= 1.0f;
			GetPlayerFacingAngle(player->getInGameID(), &rotation);
			rotation += 90.0f;
			house->setEntrance(pos[0], pos[1], pos[2], rotation);
		})
		.append("��������", std::bind(&showHouseChangePasswordDialog, player, house))
		.append("���ſ���������վ��λ��", [player, house]() {
			float pos[3];
			GetPlayerPos(player->getInGameID(), &pos[0], &pos[1], &pos[2]);
			house->setTeleportPos(pos[0], pos[1], pos[2]);
		})
		.append("���͵��ſ�", [player, house]() {
			teleportToPos(player->getInGameID(), house->_getInfo().tx, house->_getInfo().ty, house->_getInfo().tz + 1, 0, 0);
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

void showHouseChangePasswordDialog(const std::shared_ptr<Account>& player, const std::shared_ptr<House>& house)
{
	GameServer::getInstance().dialogmanager.displayInputDialog(player, "���ķ�������", "�������µ�����",
		"ȷ��", "ȡ��", false,
		[house](const std::string& newpw) { house->setPassword(newpw); }
	);
}
