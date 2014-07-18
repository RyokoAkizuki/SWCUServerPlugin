#include <sampgdk/a_players.h>

#include "HouseBuildDialog.h"
#include "GameServer.h"
#include "AccountDialogs.h"
#include "StringUtility.h"

void showHouseBuildSetOwnerDialog(const std::shared_ptr<Account>& player, const std::shared_ptr<HouseBuilder>& session)
{
	showSearchPlayerDialog(player, [player, session](const std::string& id) {
		session->setOwner(id);
		SendClientMessage(player->getInGameID(), 0xFFFFFFFF, "[HouseBuilder] �������óɹ�");
		showHouseBuildSetNameDialog(player, session);
	});
}

void showHouseBuildSetNameDialog(const std::shared_ptr<Account>& player, const std::shared_ptr<HouseBuilder>& session)
{
	GameServer::getInstance().dialogmanager.displayInputDialog(player, "���뷿������", " ",
		"ȷ��", "ȡ��", false, [player, session](const std::string& name) {
		session->setName(name);
		SendClientMessage(player->getInGameID(), 0xFFFFFFFF, STR("[HouseBuilder] ������������Ϊ " << name).c_str());
		SendClientMessage(player->getInGameID(), 0xFFFFFFFF, "[HouseBuilder] �������Ʒ�����һȦ��ȷ���߽� ����������ʼ��¼ ������Ҽ�ֹͣ��¼ ������м����");
		session->startUpdateArea();
	});
}
